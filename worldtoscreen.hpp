struct vec4 {
	float x, y, z, w;
};

// Screen coordinates
struct vec2 {
	float x, y;
};

// Player coordinates
struct vec3 {
	float x, y, z;
};

/*
	W-based W2S algorithm
*/
bool WorldToScreen(float matrix[4][4], vec3 pos, vec2& screen, int game_width, int game_height)
{
	vec4 clipCoords;
	clipCoords.x = pos.x * matrix[0][0] + pos.y * matrix[1][0] + pos.z * matrix[2][0] + matrix[3][0];
	clipCoords.y = pos.x * matrix[0][1] + pos.y * matrix[1][1] + pos.z * matrix[2][1] + matrix[3][1];
	clipCoords.z = pos.x * matrix[0][2] + pos.y * matrix[1][2] + pos.z * matrix[2][2] + matrix[3][2];
	clipCoords.w = pos.x * matrix[0][3] + pos.y * matrix[1][3] + pos.z * matrix[2][3] + matrix[3][3];

	if (clipCoords.w < 0.1f)
		return false;

	vec3 NDC;
	NDC.x = clipCoords.x / clipCoords.w;
	NDC.y = clipCoords.y / clipCoords.w;
	NDC.z = clipCoords.z / clipCoords.w;

	screen.x = (game_width / 2 * NDC.x) + (NDC.x + game_width / 2);
	screen.y = -(game_height / 2 * NDC.y) + (NDC.y + game_height / 2);
	return true;
}

/*
	Z-based W2S algorithm
*/
bool WorldToScreenZ(float g_matrix[4][4], vec3 pos, int game_width, int game_height, int& x, int& y, int& w, int& h)
{
	game_width = game_width / 2;
	game_height = game_height / 2;

	float clip_z = g_matrix[0][2] * pos.x + g_matrix[1][2] * pos.y + g_matrix[2][2] * pos.z + g_matrix[3][2];
	if (clip_z < 0.1f) {
		return false;
	}

	float clip_x = g_matrix[0][0] * pos.x + g_matrix[1][0] * pos.y + g_matrix[2][0] * pos.z + g_matrix[3][0];
	float clip_y_foot = g_matrix[0][1] * pos.x + g_matrix[1][1] * pos.y + g_matrix[2][1] * (pos.z + 25.0f) + g_matrix[3][1];
	float clip_y_head = g_matrix[0][1] * pos.x + g_matrix[1][1] * pos.y + g_matrix[2][1] * (pos.z - 50.0f) + g_matrix[3][1];

	float scal_ratio = 1.0f / clip_z;

	float screen_x = game_width + clip_x * scal_ratio * game_width;
	float screen_y_foot = game_height - clip_y_foot * scal_ratio * game_height;
	float screen_y_head = game_height - clip_y_head * scal_ratio * game_height;

	float box_height = (screen_y_foot - screen_y_head);
	w = (int)(box_height / 2.0f);
	h = (int)(box_height);
	x = (int)(screen_x - box_height / 4.0f);
	y = (int)(screen_y_head);
	return true;
}