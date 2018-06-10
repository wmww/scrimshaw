#include "epdif_display.h"

#include "epdif_driver.h"

#include "logger.h"

#include <cmath>
#include <unistd.h>
#include <memory>

EpdifDisplay::Pins const epdif_pins = {
	17, // RST
	25, // DC
	8, // CS
	24, // BUSY
};

std::unique_ptr<Display> Display::get()
{
	return std::make_unique<EpdifDisplay>(epdif_pins, epdif_logical_size, epdif_flip, epdif_swap_x_y);
}

EpdifDisplay::EpdifDisplay(Pins const& pins, Vec2i size_, Vec2<bool> flip_, bool swap_x_y_)
	: epd{std::make_unique<EpdDriver>(pins, size_, flip_, swap_x_y_)}, size{size_}
{
	last_buffer.create_empty(size);
	pending_buffer.create_empty(size);
	render_thread = std::thread([this]() {
		double const sleep_seconds = 0.1;
		double seconds_without_damage = 0;
		while (!die)
		{
			executor.iteration();
			if (should_commit)
			{
				bool buffer_damaged = last_buffer.copy_from_pixel_buffer(&pending_buffer, {});
				buffer_damaged = true;

				if (buffer_damaged)
				{
					assert_else(epd->set_mode(DisplayMode::full_update), return );
					epd->DisplayFrame();
				}
				else
				{
					// log_warning("commit called but buffer not damaged");
				}
				should_commit = false;
			}
			else
			{
				usleep(sleep_seconds * 1000000);
				seconds_without_damage += sleep_seconds;
				// if (seconds_without_damage > 4)
				//	set_mode(MODE_OFF);
			}
		}
		epd->set_mode(DisplayMode::off);
	});
}

EpdifDisplay::~EpdifDisplay()
{
	die = true;
	render_thread.join();
}

void EpdifDisplay::draw(PixelBuffer buffer, Vec2i lower_left)
{
	pending_buffers.push_back(std::make_pair(move(buffer), lower_left));
}

void EpdifDisplay::commit()
{
	executor.run(
		[this, buffers = std::make_shared<std::vector<std::pair<PixelBuffer, Vec2i>>>(move(pending_buffers))]() {
			assert_else(epd->set_mode(DisplayMode::full_update), return );
			for (auto& i : *buffers)
			{
				pending_buffer.copy_from_pixel_buffer(&i.first, i.second);
				epd->SetFrameMemory(move(i.first), i.second);
			}
			if (!buffers->empty())
			{
				should_commit = true;
			}
		});
	pending_buffers.clear();
}
