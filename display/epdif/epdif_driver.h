/**
 *  @filename   :   epd1in54.h
 *  @brief      :   Header file for e-paper display library epd1in54.cpp
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

#ifndef EPD1IN54_H
#	define EPD1IN54_H

#	include "util/vec2.h"
#include "gpio/gpio.h"
#	include "display/pixel_buffer.h"
#	include "display/epdif/epdif_display.h"

// EPD1IN54 commands
#	define DRIVER_OUTPUT_CONTROL 0x01
#	define BOOSTER_SOFT_START_CONTROL 0x0C
#	define GATE_SCAN_START_POSITION 0x0F
#	define DEEP_SLEEP_MODE 0x10
#	define DATA_ENTRY_MODE_SETTING 0x11
#	define SW_RESET 0x12
#	define TEMPERATURE_SENSOR_CONTROL 0x1A
#	define MASTER_ACTIVATION 0x20
#	define DISPLAY_UPDATE_CONTROL_1 0x21
#	define DISPLAY_UPDATE_CONTROL_2 0x22
#	define WRITE_RAM 0x24
#	define WRITE_VCOM_REGISTER 0x2C
#	define WRITE_LUT_REGISTER 0x32
#	define SET_DUMMY_LINE_PERIOD 0x3A
#	define SET_GATE_TIME 0x3B
#	define BORDER_WAVEFORM_CONTROL 0x3C
#	define SET_RAM_X_ADDRESS_START_END_POSITION 0x44
#	define SET_RAM_Y_ADDRESS_START_END_POSITION 0x45
#	define SET_RAM_X_ADDRESS_COUNTER 0x4E
#	define SET_RAM_Y_ADDRESS_COUNTER 0x4F
#	define TERMINATE_FRAME_READ_WRITE 0xFF

extern const unsigned char lut_full_update[];
extern const unsigned char lut_partial_update[];

class EpdDriver
{
public:
	Vec2i const external_size;
	Vec2i const internal_size;
	Vec2<bool> const flip;
	bool const swap_x_y;

	EpdDriver(EpdifDisplay::Pins const& pins, Vec2i size_, Vec2<bool> flip_, bool swap_x_y_);
	~EpdDriver();
	int Init(const unsigned char* lut);
	void SendCommand(unsigned char command);
	void SendData(unsigned char data);
	void WaitUntilIdle(void);
	void Reset(void);
	void SetFrameMemory(PixelBuffer buffer, Vec2i lower_left);
	void ClearFrameMemory(unsigned char color);
	void DisplayFrame(void);
	void Sleep(void);

	// returns true on success
	bool set_mode(DisplayMode new_mode);

private:
	unique_ptr<Gpio::Output> reset_pin;
	unique_ptr<Gpio::Output> dc_pin;
	unique_ptr<Gpio::Output> cs_pin;
	unique_ptr<Gpio::Input> busy_pin;
	unique_ptr<Gpio::Spi> spi_channel;
	DisplayMode mode{DisplayMode::off};
	const unsigned char* lut;

	void SetLut(const unsigned char* lut);
	void SetMemoryArea(int x_start, int y_start, int x_end, int y_end);
	void SetMemoryPointer(int x, int y);
};

#endif /* EPD1IN54_H */

/* END OF FILE */
