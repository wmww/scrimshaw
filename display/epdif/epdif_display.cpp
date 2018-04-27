#include "epdif_display.h"

#include "epd1in54.h"

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
	return std::make_unique<EpdifDisplay>(epdif_pins, epdif_size);
}

EpdifDisplay::EpdifDisplay(Pins const& pins, Vec2i const& size)
	: pins{pins},
      size{size},
      epd{std::make_unique<Epd>(pins.rst, pins.dc, pins.cs, pins.busy, size.y, size.x)} // flipped x and y are intentional
{
    render_thread = std::thread([this]() {
		while (!die)
		{
			executor.iteration();
            if (should_commit)
            {
                set_mode(MODE_FULL_UPDATE);

                assert_else(mode == MODE_FULL_UPDATE,
                            return);

                epd->DisplayFrame();
                should_commit = false;
            }
            else
            {
                usleep(0.02 * 1000000);
            }
		}
		set_mode(MODE_OFF);
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
        [this, buffers = std::make_shared<std::vector<std::pair<PixelBuffer, Vec2i>>>(move(pending_buffers))]()
        {
            set_mode(MODE_FULL_UPDATE);

            assert_else(mode == MODE_FULL_UPDATE,
                        return);

            for (auto& i: *buffers)
            {
                epd->SetFrameMemory(move(i.first), i.second);
            }
            if (!buffers->empty())
            {
                should_commit = true;
            }
        });
    pending_buffers.clear();
}

void EpdifDisplay::set_mode(Mode new_mode)
{
    if (mode == new_mode)
        return;

    if (new_mode == MODE_OFF)
    {
        epd->Sleep();
        mode = MODE_OFF;
    }
    else if (new_mode == MODE_FULL_UPDATE)
    {
        if (epd->Init(lut_full_update) == 0)
        {
            mode = MODE_FULL_UPDATE;
        }
        else
        {
            epd->Sleep();
            mode = MODE_OFF;
            log_warning("failed to set mode to MODE_FULL_UPDATE");
        }
    }
    else if (new_mode == MODE_PARTIAL_UPDATE)
    {
        if (epd->Init(lut_partial_update) == 0)
        {
            mode = MODE_PARTIAL_UPDATE;
        }
        else
        {
            epd->Sleep();
            mode = MODE_OFF;
            log_warning("failed to set mode to MODE_PARTIAL_UPDATE");
        }
    }
    else
    {
        log_warning("unknown mode");
    }
}
