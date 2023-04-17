#include "game_manager.h"

static const int game_fps = 60;

int main(void)
{
	init_game_manager();

	while (TRUE)
	{
		update_game_manager();
		render_game_manager();

		if (exit_game)
		{
			break;
		}
		Sleep(1000 / game_fps);
	}

	release_game_manager();

	return 0;
}
