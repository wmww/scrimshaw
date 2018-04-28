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
#include "epd1in54.h"

#include "util/logger.h"

Epd::~Epd(){};

Epd::Epd(EpdifDisplay::Pins const& pins_, Vec2i size_, Vec2<bool> flip_, bool swap_x_y_)
	: pins{pins_},
	  internal_size{swap_x_y_ ? Vec2i{size_.y, size_.x} : size_},
	  external_size{size_},
	  flip{flip_},
	  swap_x_y{swap_x_y_} {};

int Epd::Init(const unsigned char* lut)
{
	/* this calls the peripheral hardware interface, see epdif */
	if (IfInit(pins.reset, pins.dc, pins.busy) != 0)
	{
		return -1;
	}
	/* EPD hardware init start */
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

/**
 *  @brief: basic function for sending commands
 */
void Epd::SendCommand(unsigned char command)
{
	DigitalWrite(pins.dc, LOW);
	SpiTransfer(command);
}

/**
 *  @brief: basic function for sending data
 */
void Epd::SendData(unsigned char data)
{
	DigitalWrite(pins.dc, HIGH);
	SpiTransfer(data);
}

/**
 *  @brief: Wait until the pins.busy goes LOW
 */
void Epd::WaitUntilIdle(void)
{
	while (DigitalRead(pins.busy) == HIGH)
	{ // LOW: idle, HIGH: busy
		DelayMs(100);
	}
}

/**
 *  @brief: module reset.
 *          often used to awaken the module in deep sleep,
 *          see Epd::Sleep();
 */
void Epd::Reset(void)
{
	DigitalWrite(pins.reset, LOW); // module reset
	DelayMs(200);
	DigitalWrite(pins.reset, HIGH);
	DelayMs(200);
}

/**
 *  @brief: set the look-up table register
 */
void Epd::SetLut(const unsigned char* lut)
{
	this->lut = lut;
	SendCommand(WRITE_LUT_REGISTER);
	/* the length of look-up table is 30 bytes */
	for (int i = 0; i < 30; i++)
	{
		SendData(this->lut[i]);
	}
}

/**
 *  @brief: put an image buffer to the frame memory.
 *          this won't update the display.
 */
void Epd::SetFrameMemory(PixelBuffer buffer, Vec2i lower_left)
{
	if (swap_x_y)
		lower_left = Vec2i{lower_left.y, lower_left.x};
	if (flip.x)
		lower_left.x = internal_size.x - lower_left.x - 1;
	if (flip.y)
		lower_left.y = internal_size.y - lower_left.y - 1;

	assert_else(buffer.has_data(), return;);

	assert_else(lower_left.x >= 0 && lower_left.y >= 0, return );

	assert_else(lower_left.x % 8 == 0, { lower_left.x = (lower_left.x / 8) * 8; });

	Vec2i end = lower_left + (swap_x_y ? Vec2i{buffer.get_size().y, buffer.get_size().x} : buffer.get_size());

	assert_fatal(end.x <= internal_size.x && end.y <= internal_size.y);

	assert_else(end.x <= internal_size.x && end.y <= internal_size.y, return );

	// SetMemoryArea size is inclusive
	SetMemoryArea(lower_left.x, lower_left.y, end.x - 1, end.y - 1);
	SetMemoryPointer(lower_left.x, lower_left.y);
	SendCommand(WRITE_RAM);
	/* send the image data */
	DigitalWrite(pins.dc, HIGH);
	buffer.send_packed_bits_transformed(SpiTransfer, flip, swap_x_y);
}

/**
 *  @brief: clear the frame memory with the specified color.
 *          this won't update the display.
 */
void Epd::ClearFrameMemory(unsigned char color)
{
	SetMemoryArea(0, 0, this->internal_size.x - 1, this->internal_size.y - 1);
	SetMemoryPointer(0, 0);
	SendCommand(WRITE_RAM);
	/* send the color data */
	for (int i = 0; i < this->internal_size.x / 8 * this->internal_size.y; i++)
	{
		SendData(color);
	}
}

/**
 *  @brief: update the display
 *          there are 2 memory areas embedded in the e-paper display
 *          but once this function is called,
 *          the the next action of SetFrameMemory or ClearFrame will
 *          set the other memory area.
 */
void Epd::DisplayFrame(void)
{
	SendCommand(DISPLAY_UPDATE_CONTROL_2);
	SendData(0xC4);
	SendCommand(MASTER_ACTIVATION);
	SendCommand(TERMINATE_FRAME_READ_WRITE);
	WaitUntilIdle();
}

/**
 *  @brief: private function to specify the memory area for data R/W
 */
void Epd::SetMemoryArea(int x_start, int y_start, int x_end, int y_end)
{
	SendCommand(SET_RAM_X_ADDRESS_START_END_POSITION);
	/* x point must be the multiple of 8 or the last 3 bits will be ignored */
	SendData((x_start >> 3) & 0xFF);
	SendData((x_end >> 3) & 0xFF);
	SendCommand(SET_RAM_Y_ADDRESS_START_END_POSITION);
	SendData(y_start & 0xFF);
	SendData((y_start >> 8) & 0xFF);
	SendData(y_end & 0xFF);
	SendData((y_end >> 8) & 0xFF);
}

/**
 *  @brief: private function to specify the start point for data R/W
 */
void Epd::SetMemoryPointer(int x, int y)
{
	SendCommand(SET_RAM_X_ADDRESS_COUNTER);
	/* x point must be the multiple of 8 or the last 3 bits will be ignored */
	SendData((x >> 3) & 0xFF);
	SendCommand(SET_RAM_Y_ADDRESS_COUNTER);
	SendData(y & 0xFF);
	SendData((y >> 8) & 0xFF);
	WaitUntilIdle();
}

/**
 *  @brief: After this command is transmitted, the chip would enter the
 *          deep-sleep mode to save power.
 *          The deep sleep mode would return to standby by hardware reset.
 *          You can use Epd::Init() to awaken
 */
void Epd::Sleep()
{
	SendCommand(DEEP_SLEEP_MODE);
	WaitUntilIdle();
}

const unsigned char lut_full_update[] = {0x02, 0x02, 0x01, 0x11, 0x12, 0x12, 0x22, 0x22, 0x66, 0x69,
										 0x69, 0x59, 0x58, 0x99, 0x99, 0x88, 0x00, 0x00, 0x00, 0x00,
										 0xF8, 0xB4, 0x13, 0x51, 0x35, 0x51, 0x51, 0x19, 0x01, 0x00};

const unsigned char lut_partial_update[] = {0x10, 0x18, 0x18, 0x08, 0x18, 0x18, 0x08, 0x00, 0x00, 0x00,
											0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
											0x13, 0x14, 0x44, 0x12, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

/* END OF FILE */
