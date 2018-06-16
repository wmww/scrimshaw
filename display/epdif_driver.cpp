/**
 *  @filename   :   epd1in54.cpp
 *  @brief      :   Implements for e-paper library
 *  @author     :   Yehui from Waveshare
 *
 *  Copyright (C) Waveshare     September 5 2017
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documnetation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to  whom the Software is
 * furished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS OR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <stdlib.h>
#include "epdif_driver.h"

#include "util/logger.h"
#include "util/time.h"

EpdDriver::~EpdDriver(){};

EpdDriver::EpdDriver(EpdifDisplay::Pins const& pins, Vec2i size_, Vec2<bool> flip_, bool swap_x_y_)
	: external_size{size_},
	  internal_size{PixelBuffer::swap_if_needed(size_, swap_x_y_)},
	  flip{flip_},
	  swap_x_y{swap_x_y_},
	  reset_pin{Gpio::instance()->output(pins.reset)},
	  dc_pin{Gpio::instance()->output(pins.dc)},
	  cs_pin{Gpio::instance()->output(pins.cs)},
	  busy_pin{Gpio::instance()->input(pins.busy)},
	  spi_channel{Gpio::instance()->spi(pins.spi_channel)}
{
	log_message("created EpdDriver with new GPIO class");
}

int EpdDriver::Init(const unsigned char* lut)
{
	// EPD hardware init start
	this->lut = lut;
	Reset();
	SendCommand(DRIVER_OUTPUT_CONTROL);
	SendData((internal_size.y - 1) & 0xFF);
	SendData(((internal_size.y - 1) >> 8) & 0xFF);
	SendData(0x00); // GD = 0; SM = 0; TB = 0;
	SendCommand(BOOSTER_SOFT_START_CONTROL);
	SendData(0xD7);
	SendData(0xD6);
	SendData(0x9D);
	SendCommand(WRITE_VCOM_REGISTER);
	SendData(0xA8); // VCOM 7C
	SendCommand(SET_DUMMY_LINE_PERIOD);
	SendData(0x1A); // 4 dummy lines per gate
	SendCommand(SET_GATE_TIME);
	SendData(0x08); // 2us per line
	SendCommand(DATA_ENTRY_MODE_SETTING);
	SendData(0x03); // X increment; Y increment
	SetLut(this->lut);
	/* EPD hardware init end */
	return 0;
}

// basic function for sending commands
void EpdDriver::SendCommand(unsigned char command)
{
	dc_pin->set(false);
	spi_channel->send_byte(command);
}

// basic function for sending data
void EpdDriver::SendData(unsigned char data)
{
	dc_pin->set(true);
	spi_channel->send_byte(data);
}

void EpdDriver::WaitUntilIdle(void)
{
	// LOW: idle, HIGH: busy
	while (busy_pin->get())
	{
		sleep_for(0.1);
	}
}

// module reset.
// often used to awaken the module in deep sleep,
// see EpdDriver::Sleep();
void EpdDriver::Reset(void)
{
	reset_pin->set(false);
	sleep_for(0.2);
	reset_pin->set(true);
	sleep_for(0.2);
}

// set the look-up table register
void EpdDriver::SetLut(const unsigned char* lut)
{
	this->lut = lut;
	SendCommand(WRITE_LUT_REGISTER);
	// the length of look-up table is 30 bytes
	for (int i = 0; i < 30; i++)
	{
		SendData(this->lut[i]);
	}
}

// put an image buffer to the frame memory.
// this won't update the display.
void EpdDriver::SetFrameMemory(PixelBuffer buffer, Vec2i external_lower_left)
{
	Vec2i external_upper_right = external_lower_left + buffer.get_size();
	Vec2i external_lower_left_transformed =
		PixelBuffer::transform_to_display(external_lower_left, external_size, flip, swap_x_y);
	Vec2i external_upper_right_transformed =
		PixelBuffer::transform_to_display(external_upper_right - Vec2i{1, 1}, external_size, flip, swap_x_y);
	Vec2i internal_lower_left = Vec2i{std::min(external_lower_left_transformed.x, external_upper_right_transformed.x),
									  std::min(external_lower_left_transformed.y, external_upper_right_transformed.y)};
	Vec2i internal_upper_right =
		Vec2i{std::max(external_lower_left_transformed.x, external_upper_right_transformed.x),
			  std::max(external_lower_left_transformed.y, external_upper_right_transformed.y)} +
		Vec2i{1, 1};

	assert_else(buffer.has_data(), return;);

	assert_else(internal_lower_left.x >= 0 && internal_lower_left.y >= 0,
				log_warning("internal_lower_left: " + internal_lower_left.to_string());
				return );

	assert_else(internal_lower_left.x % 8 == 0, { internal_lower_left.x = (internal_lower_left.x / 8) * 8; });

	assert_else(internal_upper_right.x <= internal_size.x && internal_upper_right.y <= internal_size.y, return );

	// SetMemoryArea size is inclusive
	SetMemoryArea(internal_lower_left.x, internal_lower_left.y, internal_upper_right.x - 1, internal_upper_right.y - 1);
	SetMemoryPointer(internal_lower_left.x, internal_lower_left.y);
	SendCommand(WRITE_RAM);
	/* send the image data */
	dc_pin->set(true);
	auto data = buffer.pack_bits_transformed(flip, swap_x_y);
	for (size_t i = 0; i < data.second; i++)
	{
		spi_channel->send_byte(*(data.first.get() + i));
	}
}

// this won't update the display.
void EpdDriver::ClearFrameMemory(unsigned char color)
{
	SetMemoryArea(0, 0, this->internal_size.x - 1, this->internal_size.y - 1);
	SetMemoryPointer(0, 0);
	SendCommand(WRITE_RAM);
	for (int i = 0; i < this->internal_size.x / 8 * this->internal_size.y; i++)
	{
		SendData(color);
	}
}

// update the display
// there are 2 memory areas embedded in the e-paper display
// but once this function is called,
// the the next action of SetFrameMemory or ClearFrame will
// set the other memory area.
void EpdDriver::DisplayFrame(void)
{
	SendCommand(DISPLAY_UPDATE_CONTROL_2);
	SendData(0xC4);
	SendCommand(MASTER_ACTIVATION);
	SendCommand(TERMINATE_FRAME_READ_WRITE);
	WaitUntilIdle();
}

// private function to specify the memory area for data R/W
void EpdDriver::SetMemoryArea(int x_start, int y_start, int x_end, int y_end)
{
	SendCommand(SET_RAM_X_ADDRESS_START_END_POSITION);
	// x point must be the multiple of 8 or the last 3 bits will be ignored
	SendData((x_start >> 3) & 0xFF);
	SendData((x_end >> 3) & 0xFF);
	SendCommand(SET_RAM_Y_ADDRESS_START_END_POSITION);
	SendData(y_start & 0xFF);
	SendData((y_start >> 8) & 0xFF);
	SendData(y_end & 0xFF);
	SendData((y_end >> 8) & 0xFF);
}

// private function to specify the start point for data R/W
void EpdDriver::SetMemoryPointer(int x, int y)
{
	SendCommand(SET_RAM_X_ADDRESS_COUNTER);
	// x point must be the multiple of 8 or the last 3 bits will be ignored
	SendData((x >> 3) & 0xFF);
	SendCommand(SET_RAM_Y_ADDRESS_COUNTER);
	SendData(y & 0xFF);
	SendData((y >> 8) & 0xFF);
	WaitUntilIdle();
}

// After this command is transmitted, the chip would enter the
// deep-sleep mode to save power.
// The deep sleep mode would return to standby by hardware reset.
// You can use EpdDriver::Init() to awaken
void EpdDriver::Sleep()
{
	SendCommand(DEEP_SLEEP_MODE);
	WaitUntilIdle();
}

bool EpdDriver::set_mode(DisplayMode new_mode)
{
	if (new_mode == DisplayMode::off)
	{
		Sleep();
		mode = DisplayMode::off;
	}
	else if (new_mode == DisplayMode::full_update)
	{
		if (Init(lut_full_update) == 0)
		{
			mode = DisplayMode::full_update;
		}
		else
		{
			Sleep();
			mode = DisplayMode::off;
			log_warning("failed to set mode to DisplayMode::full_update");
		}
	}
	else if (new_mode == DisplayMode::partial_update)
	{
		if (Init(lut_partial_update) == 0)
		{
			mode = DisplayMode::partial_update;
		}
		else
		{
			Sleep();
			mode = DisplayMode::off;
			log_warning("failed to set mode to DisplayMode::partial_update");
		}
	}
	else
	{
		log_warning("unknown mode");
	}

	return mode == new_mode;
}

const unsigned char lut_full_update[] = {0x02, 0x02, 0x01, 0x11, 0x12, 0x12, 0x22, 0x22, 0x66, 0x69,
										 0x69, 0x59, 0x58, 0x99, 0x99, 0x88, 0x00, 0x00, 0x00, 0x00,
										 0xF8, 0xB4, 0x13, 0x51, 0x35, 0x51, 0x51, 0x19, 0x01, 0x00};

const unsigned char lut_partial_update[] = {0x10, 0x18, 0x18, 0x08, 0x18, 0x18, 0x08, 0x00, 0x00, 0x00,
											0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
											0x13, 0x14, 0x44, 0x12, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

/* END OF FILE */
