#include "main.h"

/*
	Read object properties from json files
*/
bool getObjects(LIST_OBJECTS &L, std::string filename) {
	//Get json values
	Json::Value json_obj;
	std::ifstream json_file(filename, std::ifstream::binary);
	if (!json_file.is_open() || json_file.bad()) {
		return false;
	}
	json_file >> json_obj;
	json_file.close();

	//Get obstacle values
	float o_x = 0.0f, o_y = 0.0f, o_dir = 0.0f, temp_i, temp_j, rot_i, rot_j, half_width = obj_obstacle_w / 2.0f, half_length = obj_obstacle_w / 2.0f;
	for (unsigned int a = 0; a < json_obj["Obstacle"].size(); a++) {
		o_x = std::stof(json_obj["Obstacle"][a]["x_pos"].asString());
		o_y = mirrorI(std::stof(json_obj["Obstacle"][a]["y_pos"].asString()));
		L.obj_obstacle[a].x_pos = o_x;
		L.obj_obstacle[a].y_pos = o_y;
		L.num_obstacle++;
		//Calculate corner positions
		L.obj_obstacle[a].corner[0] = makePoint(o_y - half_width, o_x + half_length);
		L.obj_obstacle[a].corner[1] = makePoint(o_y - half_width, o_x - half_length);
		L.obj_obstacle[a].corner[2] = makePoint(o_y + half_width, o_x - half_length);
		L.obj_obstacle[a].corner[3] = makePoint(o_y + half_width, o_x + half_length);
		for (unsigned int b = 0; b < 4; b++) {
			temp_i = L.obj_obstacle[a].corner[b].i - o_y;
			temp_j = L.obj_obstacle[a].corner[b].j - o_x;
			rot_i = (temp_i * cos(o_dir)) - (temp_j * sin(o_dir));
			rot_j = (temp_j * cos(o_dir)) + (temp_i * sin(o_dir));
			L.obj_obstacle[a].corner[b].i = rot_i + o_y;
			L.obj_obstacle[a].corner[b].j = rot_j + o_x;
		}
	}

	//Get cube values
	half_width = obj_cube_w / 2.0f, half_length = obj_cube_w / 2.0f;
	for (unsigned int a = 0; a < json_obj["Cube"].size(); a++) {
		o_x = std::stof(json_obj["Cube"][a]["x_pos"].asString());
		o_y = mirrorI(std::stof(json_obj["Cube"][a]["y_pos"].asString()));
		o_dir = std::stof(json_obj["Cube"][a]["direction"].asString());
		L.obj_cube[a].x_pos = o_x;
		L.obj_cube[a].y_pos = o_y;
		L.obj_cube[a].dir = o_dir;
		L.obj_cube[a].letter = json_obj["Cube"][a]["letter"].asString().at(0);
		L.num_cube++;
		//Calculate corner positions
		L.obj_cube[a].corner[0] = makePoint(o_y - half_width, o_x + half_length);
		L.obj_cube[a].corner[1] = makePoint(o_y - half_width, o_x - half_length);
		L.obj_cube[a].corner[2] = makePoint(o_y + half_width, o_x - half_length);
		L.obj_cube[a].corner[3] = makePoint(o_y + half_width, o_x + half_length);
		for (unsigned int b = 0; b < 4; b++) {
			temp_i = L.obj_cube[a].corner[b].i - o_y;
			temp_j = L.obj_cube[a].corner[b].j - o_x;
			rot_i = (temp_i * cos(o_dir)) - (temp_j * sin(o_dir));
			rot_j = (temp_j * cos(o_dir)) + (temp_i * sin(o_dir));
			L.obj_cube[a].corner[b].i = rot_i + o_y;
			L.obj_cube[a].corner[b].j = rot_j + o_x;
		}
	}

	//Get mothership values
	half_width = obj_mothership_w / 2.0f, half_length = obj_mothership_l / 2.0f;
	o_x = std::stof(json_obj["Mothership"][0]["x_pos"].asString());
	o_y = mirrorI(std::stof(json_obj["Mothership"][0]["y_pos"].asString()));
	o_dir = std::stof(json_obj["Mothership"][0]["direction"].asString());
	L.obj_mothership.x_pos = o_x;
	L.obj_mothership.y_pos = o_y;
	L.obj_mothership.dir = o_dir;
	//L.obj_mothership.rad = std::stof(json_obj["Mothership"][0]["radius"].asString());
	//Calculate corner positions
	L.obj_mothership.corner[0] = makePoint(o_y - half_width, o_x + half_length);
	L.obj_mothership.corner[1] = makePoint(o_y - half_width, o_x - half_length);
	L.obj_mothership.corner[2] = makePoint(o_y + half_width, o_x - half_length);
	L.obj_mothership.corner[3] = makePoint(o_y + half_width, o_x + half_length);
	for (unsigned int b = 0; b < 4; b++) {
		temp_i = L.obj_mothership.corner[b].i - o_y;
		temp_j = L.obj_mothership.corner[b].j - o_x;
		rot_i = (temp_i * cos(o_dir)) - (temp_j * sin(o_dir));
		rot_j = (temp_j * cos(o_dir)) + (temp_i * sin(o_dir));
		L.obj_mothership.corner[b].i = rot_i + o_y;
		L.obj_mothership.corner[b].j = rot_j + o_x;
	}

	return true;
}

/*
	Create the terrain grid from the objects in the field
*/
void makeTerrainGrid(LIST_OBJECTS &L, uint8_t terrainGrid[][GRID_W + 1]) {
	int16_t gridI, gridJ, swap = 0, draw, nodes[24] = { INT16_MAX };
	uint8_t c0, c1, o, num_nodes = 0;
	float pixI, pixJ, c0_i, c0_j, c1_i, c1_j;

	//Build a list of nodes
	for (gridI = 0; gridI < GRID_H; gridI++) {
		#ifdef _DEBUG
		std::cout << "\tRow=" << gridI << std::endl;
		#endif
		//Get the pixel value of the current grid row
		pixI = gridI * GRID;

		//Reset values
		num_nodes = 0;
		for (gridJ = 0; gridJ < 24; gridJ++) { nodes[gridJ] = UINT16_MAX; }

		//Search for intersections with the mothership
		#ifdef _DEBUG
		std::cout << "\t\tMothership:" << std::endl;
		#endif
		c1 = CORNER - 1;
		for (c0 = 0; c0 < CORNER; c0++) {
			c0_i = L.obj_mothership.corner[c0].i;
			c0_j = L.obj_mothership.corner[c0].j;
			c1_i = L.obj_mothership.corner[c1].i;
			c1_j = L.obj_mothership.corner[c1].j;
			//Check if this row intersects the line between c0 and c1
			if ((pixI < c0_i && pixI > c1_i) ||
				(pixI > c0_i && pixI < c1_i)) {
				#ifdef _DEBUG
				std::cout << "\t\t\tINTERSECTION-> c0(" << (int16_t)c0 << "), c1(" << (int16_t)c1 << ")" << std::endl;
				#endif
				//Calculate the x position at the intersection
				pixJ = c0_j + ((pixI - c0_i) * (c1_j - c0_j)) / (c1_i - c0_i);
				//Convert that to a gridcell
				gridJ = int16_t(floor(pixJ / GRID));
				nodes[num_nodes] = gridJ;
				num_nodes++;
			}
			c1 = c0;
		}

		//Search for intersections with cubes
		for (o = 0; o < L.num_cube; o++) {
			#ifdef _DEBUG
			std::cout << "\t\tCube" << (int16_t)o << ":" << std::endl;
			#endif
			c1 = CORNER-1;
			for (c0 = 0; c0 < CORNER; c0++) {
				c0_i = L.obj_cube[o].corner[c0].i;
				c0_j = L.obj_cube[o].corner[c0].j;
				c1_i = L.obj_cube[o].corner[c1].i;
				c1_j = L.obj_cube[o].corner[c1].j;
				//Check if this row intersects the line between c0 and c1
				if ((pixI < c0_i && pixI > c1_i) ||
					(pixI > c0_i && pixI < c1_i)) {
					#ifdef _DEBUG
					std::cout << "\t\t\tINTERSECTION-> c0(" << (int16_t)c0 << "), c1(" << (int16_t)c1 << ")" << std::endl;
					#endif
					//Calculate the x position at the intersection
					pixJ = c0_j + ((pixI - c0_i) * (c1_j - c0_j)) / (c1_i - c0_i);
					//Convert that to a gridcell
					gridJ = int16_t(floor(pixJ / GRID));
					nodes[num_nodes] = gridJ;
					num_nodes++;
				}
				c1 = c0;
			}
		}

		//Search for intersections with obstacles
		for (o = 0; o < L.num_obstacle; o++) {
			#ifdef _DEBUG
			std::cout << "\t\tObstacle" << (int16_t)o << ":" << std::endl;
			#endif
			c1 = CORNER-1;
			for (c0 = 0; c0 < CORNER; c0++) {
				c0_i = L.obj_obstacle[o].corner[c0].i;
				c0_j = L.obj_obstacle[o].corner[c0].j;
				c1_i = L.obj_obstacle[o].corner[c1].i;
				c1_j = L.obj_obstacle[o].corner[c1].j;
				//Check if this row intersects the line between c0 and c1
				if ((pixI < c0_i && pixI > c1_i) ||
					(pixI > c0_i && pixI < c1_i)) {
					#ifdef _DEBUG
					std::cout << "\t\t\tINTERSECTION-> c0(" << (int16_t)c0 << "), c1(" << (int16_t)c1 << ")" << std::endl;
					#endif
					//Calculate the x position at the intersection
					pixJ = c0_j + ((pixI - c0_i) * (c1_j - c0_j)) / (c1_i - c0_i);
					//Convert that to a gridcell
					gridJ = int16_t(floor(pixJ / GRID));
					nodes[num_nodes] = gridJ;
					num_nodes++;
				}
				c1 = c0;
			}
		}

		//Fill in node pairs
		for (o = 0; o < num_nodes; o += 2) {
			#ifdef _DEBUG
			std::cout << "\t\tFilling from " << nodes[o] << " - " << nodes[o + 1] << std::endl;
			#endif
			//Clamp the values to the width of the field
			nodes[o] = bound(nodes[o], int16_t(0), int16_t(GRID_W - 1));
			nodes[o + 1] = bound(nodes[o + 1], int16_t(0), int16_t(GRID_W - 1));
			//Reorder the pair from smallest to largest
			if (nodes[o] > nodes[o + 1]) {
				swap = nodes[o];
				nodes[o] = nodes[o + 1];
				nodes[o + 1] = swap;
			}
			//Fill the space between them
			for (gridJ = nodes[o]; gridJ <= nodes[o + 1]; gridJ++) {
				terrainGrid[gridI][gridJ] = 255;
			}
			//Mark the row as populated in the data column
			terrainGrid[gridI][GRID_W] = 255;
		}
	}

	//Make safety clouds around objects
	int16_t grid_i, grid_j;
	//Make radius around cubes
	#ifdef _DEBUG
	std::cout << "\t\tMaking cubes radius" << std::endl;
	#endif
	for (o = 0; o < L.num_cube; o++) {
		grid_i = int16_t(L.obj_cube[o].y_pos / GRID);
		grid_j = int16_t(L.obj_cube[o].x_pos / GRID);
		draw = int16_t(min(obj_robot_l / 2.0f, obj_robot_w / 2.0f) / GRID);
		drawFilledCircle(terrainGrid, makePoint(grid_i, grid_j), draw + int16_t(6 / GRID), 127);
		drawFilledCircle(terrainGrid, makePoint(grid_i, grid_j), draw, 255);
	}
	//Make radius around obstacles
	#ifdef _DEBUG
	std::cout << "\t\tMaking obstacle radius" << std::endl;
	#endif
	for (o = 0; o < L.num_obstacle; o++) {
		grid_i = int16_t(L.obj_obstacle[o].y_pos / GRID);
		grid_j = int16_t(L.obj_obstacle[o].x_pos / GRID);
		draw = int16_t(min(obj_robot_l / 2.0f, obj_robot_w / 2.0f) / GRID);
		drawFilledCircle(terrainGrid, makePoint(grid_i, grid_j), draw + int16_t(6 / GRID), 127);
		drawFilledCircle(terrainGrid, makePoint(grid_i, grid_j), draw, 255);
	}

}

/*
	Find the shortest path from src to dest
*/
void aStar(uint8_t terrainGrid[][GRID_W + 1], point<int16_t> src, point<int16_t> dest, std::vector<point<int16_t>> &path) {
	//Initialize list of points to visit
	PQueue<point<int16_t>, int16_t> visit;
	visit.put(src, 0);

	//Initialize visited & cost trackers
	std::unordered_map<point<int16_t>, point<int16_t>> parents;
	std::unordered_map<point<int16_t>, int16_t> curr_cost;

	//Check if points are valid & unblocked
	if (!pointValid(src)) {
		std::cout << "\tWARNING: Source invalid" << std::endl;
		return;
	}
	if (!pointValid(dest)) {
		std::cout << "\tWARNING: Destination invalid" << std::endl;
		return;
	}
	if (terrainGrid[src.i][src.j] == 255) {
		std::cout << "\tWARNING: Source blocked" << std::endl;
		return;
	}
	if (terrainGrid[dest.i][dest.j] == 255) {
		std::cout << "\tWARNING: Destination blocked" << std::endl;
		return;
	}
	if (src == dest) {
		std::cout << "\tWARNING: Source = Destination" << std::endl;
		return;
	}

	//Search through points
	while (!visit.empty()) {
		//Get the current lowest cost point
		point<int16_t> p = visit.get();

		//Check if point is destination
		if (p == dest) {
			//Form a stack of points by following the parents
			while (p != src) {
				path.insert(path.begin(), p);
				p = parents[p];
			}
			path.insert(path.begin(), src);

			//DEBUG report total memory usage of pathfinding objects
			#ifdef _DEBUG
			size_t count = 0;
			for (unsigned int m = 0; m < parents.bucket_count(); ++m) {
				size_t bucket_size = parents.bucket_size(m);
				if (bucket_size == 0) {
					count++;
				}
				else {
					count += bucket_size;
				}
			}
			std::cout << "\tParent Map memory usage: " << count * sizeof(point<int16_t>) << " bytes" << std::endl;
			count = 0;
			for (unsigned int m = 0; m < curr_cost.bucket_count(); ++m) {
				size_t bucket_size = curr_cost.bucket_size(m);
				if (bucket_size == 0) {
					count++;
				}
				else {
					count += bucket_size;
				}
			}
			std::cout << "\tCost Map memory usage: " << count * sizeof(int16_t) << " bytes" << std::endl;
			std::cout << "\tPoint Stack memory usage: " << path.size() * sizeof(point<int16_t>) << " bytes" << std::endl;
			std::cout << "\tTerrain Grid memory usage: " << GRID_W * GRID_H * sizeof(uint8_t) << " bytes" << std::endl;
			#endif
			return;
		}

		//Search points neighbors
		for (uint8_t n = 0; n < 8; n++) {
			//Calculate next neighbor to check
			int16_t new_i = p.i + calculateSequence(n);
			int16_t new_j = p.j + calculateSequence(n + 6);
			point<int16_t> new_p(new_i, new_j);

			//Check if neighbor is valid & unblocked
			if (!pointValid(new_p) || terrainGrid[new_i][new_j] == 255) {
				continue;
			}

			//Calculate cost of moving to neighbor
			int16_t new_cost = curr_cost[p] + 1;

			//Add neighbor if it's not on the list, or the cost of moving to
			//it from the current point is lower than it was from another point
			if (curr_cost.find(new_p) == curr_cost.end() || new_cost < curr_cost[new_p]) {
				curr_cost[new_p] = new_cost;
				int16_t priority = new_cost + calculateH(new_p, dest);
				visit.put(new_p, priority);
				parents[new_p] = p;
			}
		}
	}
}

/*
	Fill the circle defined by center c and radius r with value v
*/
void drawFilledCircle(uint8_t terrainGrid[][GRID_W + 1], point<int16_t> c, int16_t r, uint8_t v) {
	//Draw an octant using a modded Midpoint algorithm
	int16_t j = r, i = 0, d = 0, draw_i0, draw_i1, draw_j0, draw_j1;
	while (j >= i) {
		//Fill the pixel rows
		for (int16_t p = -j; p <= j; p++) {
			//Bound the coordinates to the grid
			draw_i0 = bound(c.i - i, 0, GRID_H - 1);
			draw_i1 = bound(c.i + i, 0, GRID_H - 1);
			draw_j0 = bound(c.j + p, 0, GRID_W - 1);

			//Fill the octant
			terrainGrid[draw_i0][draw_j0] = v;
			terrainGrid[draw_i1][draw_j0] = v;
		}
		for (int16_t p = -i; p <= i; p++) {
			//Bound the coordinates to the grid
			draw_i0 = bound(c.i - j, 0, GRID_H - 1);
			draw_i1 = bound(c.i + j, 0, GRID_H - 1);
			draw_j0 = bound(c.j + p, 0, GRID_W - 1);

			terrainGrid[draw_i0][draw_j0] = v;
			terrainGrid[draw_i1][draw_j0] = v;
		}
		//Populate the data column
		draw_i0 = bound(c.i - i, 0, GRID_H - 1);
		draw_i1 = bound(c.i + i, 0, GRID_H - 1);
		draw_j0 = bound(c.i - j, 0, GRID_W - 1);
		draw_j1 = bound(c.i + j, 0, GRID_W - 1);

		terrainGrid[draw_i0][GRID_W] = v;
		terrainGrid[draw_i1][GRID_W] = v;
		terrainGrid[draw_j0][GRID_W] = v;
		terrainGrid[draw_j1][GRID_W] = v;

		//Check which value to modify
		if (d <= 0) {
			i++;
			d += (2 * i) + 1;
		}
		else {
			j--;
			d -= (2 * j) + 1;
		}

	}
	return;
}

/*
	Make a group of debug files to diagnose issues
*/
bool testTerrainGrid(LIST_OBJECTS &L, uint8_t terrainGrid[][GRID_W + 1], std::stack<point<float>> &path) {
	//Empty the path queue into a vector for easier searching
	int16_t path_high = GRID_H, path_low = 0, gridI;
	std::vector<point<float>> path_vec;
	while (!path.empty()) {
		path_vec.push_back(path.top());
		//Mark the highest & lowest part of the path
		path_high = min(path_high, int16_t(path.top().i));
		path_low = max(path_low, int16_t(path.top().i));
		//Mark the rows containing path nodes in the data column
		gridI = int16_t(path.top().i / GRID);
		terrainGrid[gridI][GRID_W] = 255;
		path.pop();
	}

	//Output text object property text file
	std::ofstream out_text("pathout/objects.txt");
	if (!out_text.is_open() || out_text.bad()) {
		return false;
	}
	out_text << "Mothership:" << std::endl;
	for (int i = 0; i < 4; i++) {
		out_text << "\tCorner" << i << "_x=" << L.obj_mothership.corner[i].j << std::endl
			<< "\tCorner" << i << "_y=" << L.obj_mothership.corner[i].i << std::endl;
	}
	out_text << "Path:" << std::endl;
	for (unsigned int s = 0; s < path_vec.size(); s++) {
		out_text << "\ty = " << mirrorI(path_vec.at(s).i) << "cm, x = " << path_vec.at(s).j << "cm, rot = " << path_vec.at(s).d << " rad" << std::endl;
	}
	out_text.close();

	//Draw terrain grid
	/*
	std::ofstream out_file("output/terrain.ppm");
	if (!out_file.is_open() || out_file.bad()) {
		return false;
	}
	uint8_t r = 0, g = 0, b = 0;
	out_file << "P3\n" << GRID_W << " " << GRID_H << "\n255\n";
	for (int i = 0; i < GRID_H; i++) {
		//Check the data column to see if the row is empty
		if (terrainGrid[i][GRID_W] == 0) {
			//Fill in a long line of black
			std::string black = "0 0 0\n";
			std::string out;
			for (unsigned int num = 0; num < GRID_W; num++) {
				out += black;
			}
			out_file.write(out.c_str(), out.size());
		}
		else {
			for (int j = 0; j < GRID_W; j++) {
				//Get the point color
				r = terrainGrid[i][j];
				g = b = r;
				//Make it green if it's part of the path
				if (i >= path_high && i <= path_low) {
					for (unsigned int s = 0; s < path_vec.size(); s++) {
						if (path_vec.at(s).i == i && path_vec.at(s).j == j) {
							path_vec.erase(path_vec.begin() + s);
							g = 255;
							r = b = 0;
							break;
						}
					}
				}
				out_file << int(r) << " " << int(g) << " " << int(b) << "\n";
			}
		}
	}
	out_file.close();
	*/

	return true;
}

/*
	Calculate the nth number of a sequence that is used to determine which successor to analyze next
*/
int16_t calculateSequence(int16_t n) {
	if (n % 4 == 0) { return 0; }
	else if (n % 8 < 4) { return 1; }
	else { return -1; }
}

/*
	Find the optimal destination point around the given cube & generate a path to it
*/
void makePath(LIST_OBJECTS &L, uint8_t terrainGrid[][GRID_W + 1], point<int16_t> src, PROP_CUBE &c, std::stack<point<float>> &path) {
	unsigned int a, b;
	int8_t safe = -1;
	int16_t draw, score[4] = { INT16_MAX };
	float theta = c.dir, delta = 0.01f;
	float r = (obj_cube_w / 2.0f) + (obj_robot_l / 2.0f) + delta;
	point<int16_t> checks[4];
	point<float> corners[4], temp_p;

	//Generate a list of points around the cube to check
	for (a = 0; a < 4; a++) {
		checks[a].i = int16_t((c.y_pos + (r * sin(theta))) / GRID);
		checks[a].j = int16_t((c.x_pos + (r * cos(theta))) / GRID);
		checks[a].d = theta + PI;
		theta += (PI / 2.0f);
	}

	//Determine which point is the safest
	for (a = 0; a < 4; a++) {
		//Calculate the corners of the robot
		calculateCorners(checks[a], obj_robot_w, obj_robot_l, corners);

		//Calculate the safety of that region
		score[a] = calculateArea(terrainGrid, corners) + calculateH(src, checks[a]);
		if (safe == -1 || score[a] < score[safe]) {
			safe = a;
		}
	}

	//Generate a path from src to the safest point
	if (safe == -1) {
		std::cout << "[ERROR] Could not find safe point around cube " << c.letter << std::endl;
		return;
	}
	std::vector<point<int16_t>> raw_path;
	aStar(terrainGrid, src, checks[safe], raw_path);

	//Calculate angles along the path
	int16_t edge_buff = int16_t(obj_robot_w / (2.0f * GRID));
	float closest = FLT_MAX;
	for (std::vector<point<int16_t>>::reverse_iterator it = raw_path.rbegin(); it != raw_path.rend(); it++) {
		/*
			The first thing that has to happen with angle checking is setting the current points rotation to
			the value of the previous iterator (which represents the next closest point to the destination
			in the path). The reason for this is so that any angle adjustments propogate backwards through
			the path, so that critical rotations will happen before they are necessary.
		*/
		if (it != raw_path.rbegin()) {
			std::vector<point<int16_t>>::reverse_iterator last_it = it - 1;
			it->d = last_it->d;
		}

		//Only do a rotational check if the robot is significantly close enough to danger
		for (b = 0; b < L.num_obstacle; b++) {
			closest = min(closest, float(sqrt(pow(L.obj_obstacle[b].y_pos - it->i, 2) + pow(L.obj_obstacle[b].x_pos - it->j, 2))));
		}
		for (b = 0; b < L.num_cube; b++) {
			closest = min(closest, float(sqrt(pow(L.obj_cube[b].y_pos - it->i, 2) + pow(L.obj_cube[b].x_pos - it->j, 2))));
		}
		if (it->i < edge_buff || it->i > GRID_H - edge_buff
			|| it->j < edge_buff || it->j > GRID_W - edge_buff
			|| closest < (edge_buff * 2.0f)) {
			//Check if the current angle of the robot is too close to danger
			calculateCorners(*it, obj_robot_w, obj_robot_l, corners);
			if (calculateArea(terrainGrid, corners) > 0) {
				//Check surrounding angles for a better position
				float optimal_dir = it->d, max_dir = optimal_dir + PI, curr_dir = optimal_dir + 0.1f, area, min_area = FLT_MAX;
				point<int16_t> temp_p = makePoint(it->i, it->j, it->d);
				for (curr_dir; curr_dir < max_dir; curr_dir += delta) {
					//Update the point to check
					temp_p.d = curr_dir;
					//Get the corners at this angle
					calculateCorners(temp_p, obj_robot_w, obj_robot_l, corners);
					//Check the area of this angle
					area = calculateArea(terrainGrid, corners);
					//Record it if it's optimal
					if (area < min_area) {
						min_area = area;
						optimal_dir = curr_dir;
						//Stop looking if you find a rotation completely free from danger
						if (area == 0.0f) {
							break;
						}
					}
				}
				//Save this optimal new direction
				it->d = optimal_dir;
			}
		}
	}

	//Remove the cube from the field in the robots mind
	point<int16_t> c_center = makePoint(int16_t(c.y_pos / GRID), int16_t(c.x_pos / GRID));
	draw = int16_t(min(obj_robot_l / 2.0f, obj_robot_w / 2.0f) / GRID);
	drawFilledCircle(terrainGrid, c_center, draw + int16_t(6 / GRID), 0);
	uint8_t index = L.num_cube;
	for (a = 0; a < (unsigned int)(L.num_cube - 1); a++) {
		//Shift back array elements
		if (index < L.num_cube) {
			L.obj_cube[a] = L.obj_cube[a + 1];
		}
		//Found the cube to delete
		if (L.obj_cube[a] == c) {
			index = a;
			L.obj_cube[a] = L.obj_cube[a + 1];
		}
	}
	L.num_cube--;

	//Turn the vector into a stack of points
	int16_t di, dj, last_di = INT16_MAX, last_dj = INT16_MAX;
	float last_rot = FLT_MAX;
	for (std::vector<point<int16_t>>::reverse_iterator it = raw_path.rbegin(); it != raw_path.rend(); it++) {
		//Check if the end of the path is reached
		if (it + 1 == raw_path.rend()) {
			//Convert grid coordinates to CM
			point<float> p = makePoint(float(it->i * GRID), float(it->j * GRID));
			path.push(p);

			//If the end is reached, skip the next point conversion
			break;
		}

		//Check if the point is different from the ones before, or if it is the last point in the list
		di = (it++)->i - it->i;
		dj = (it++)->j - it->j;
		if (di != last_di || dj != last_dj || it->d != last_rot) {
			//Convert grid coordinates to CM
			point<float> p = makePoint(float(it->i * GRID), float(it->j * GRID));
			path.push(p);
		}

		//Save the current calculations for the next cycle
		last_di = di;
		last_dj = dj;
		last_rot = it->d;
	}

	return;
}

/*
	Calculate the corners of the shape given py point p and dimensions w and l
	Returns values by reference in corners
*/
void calculateCorners(point<int16_t> p, float w, float l, point<float> (&corners)[4]) {
	float temp_i, temp_j, rot_i, rot_j;
	//Initialize the corner values
	corners[0] = makePoint((p.i * GRID) - (w / 2.0f), (p.j * GRID) + (l / 2.0f));
	corners[1] = makePoint((p.i * GRID) - (w / 2.0f), (p.j * GRID) - (l / 2.0f));
	corners[2] = makePoint((p.i * GRID) + (w / 2.0f), (p.j * GRID) - (l / 2.0f));
	corners[3] = makePoint((p.i * GRID) + (w / 2.0f), (p.j * GRID) + (l / 2.0f));

	//Adjust based on the rotation component of p
	for (unsigned int b = 0; b < 4; b++) {
		temp_i = corners[b].i - (p.i * GRID);
		temp_j = corners[b].j - (p.j * GRID);
		rot_i = (temp_i * cos(p.d)) - (temp_j * sin(p.d));
		rot_j = (temp_j * cos(p.d)) + (temp_i * sin(p.d));
		corners[b].i = rot_i + (p.i * GRID);
		corners[b].j = rot_j + (p.j * GRID);
	}
}

/*
	Calculate the sum of all values of the terrain grid bound by corners
*/
int16_t calculateArea(uint8_t terrainGrid[][GRID_W + 1], point<float> (&corners)[4]) {
	uint8_t c0 = 0, c1 = 0, o = 0, num_nodes = 0;
	int16_t gridI_min = -1, gridI_max = -1, gridJ_min = -1, gridJ_max = -1, gI = -1, gJ = -1, sum = 0, swap = -1, nodes[2] = { 0 };
	float pixI, pixJ, c0_i, c0_j, c1_i, c1_j;
	//Get the bounds of the search
	gridI_min = int16_t(floor(min(corners[0].i, corners[1].i, corners[2].i, corners[3].i) * GRID));
	gridI_max = int16_t(ceil(max(corners[0].i, corners[1].i, corners[2].i, corners[3].i) * GRID));
	gridJ_min = int16_t(floor(min(corners[0].j, corners[1].j, corners[2].j, corners[3].j) * GRID));
	gridJ_max = int16_t(ceil(max(corners[0].j, corners[1].j, corners[2].j, corners[3].j) * GRID));

	//Search grid rows
	for (gI = gridI_min; gI < gridI_max; gI++) {
		//Get the pixel value of the current row
		pixI = gI * GRID;

		//Reset values
		num_nodes = 0;
		nodes[0] = nodes[1] = INT16_MAX;

		//Check for intersections with the corners
		c1 = CORNER - 1;
		for (c0 = 0; c0 < CORNER; c0++) {
			c0_i = corners[c0].i;
			c0_j = corners[c0].j;
			c1_i = corners[c1].i;
			c1_j = corners[c1].j;
			//Check if this row intersects the line between c0 and c1
			if ((pixI < c0_i && pixI > c1_i) ||
				(pixI > c0_i && pixI < c1_i)) {
				//Calculate the x position at the intersection
				pixJ = c0_j + ((pixI - c0_i) * (c1_j - c0_j)) / (c1_i - c0_i);
				//Convert that to a gridcell
				gJ = int16_t(floor(pixJ / GRID));
				nodes[num_nodes] = gJ;
				num_nodes++;
			}
			c1 = c0;
		}

		//Clamp the nodes to the width of the field
		nodes[0] = bound(nodes[0], (int16_t)0, (int16_t)(GRID_W - 1));
		nodes[1] = bound(nodes[1], (int16_t)0, (int16_t)(GRID_W - 1));
		//Reorder the pair from smallest to largest
		if (nodes[0] > nodes[1]) {
			swap = nodes[0];
			nodes[0] = nodes[1];
			nodes[1] = swap;
		}
		//Check the space between the nodes
		for (gJ = nodes[0]; gJ <= nodes[1]; gJ++) {
			sum += terrainGrid[gI][gJ];
		}
	}

	return sum;
}

/*
	Output the path stack as a comma seperated value list
*/
bool outputPath(std::stack<point<float>> &path) {
	std::ofstream out_text("path.txt");
	if (!out_text.is_open() || out_text.bad() || path.size() == 0) {
		return false;
	}
	while(!path.empty()) {
		point<float> temp_p = path.top();
		out_text << temp_p.j << "," << mirrorI(temp_p.i) << "," << temp_p.d << std::endl;
		path.pop();
	}
	out_text.close();
	return true;
}

int main() {
	//Inititalize
	LIST_OBJECTS obj_list;
	std::stack<point<float>> path;
	uint8_t terrainGrid[GRID_H][GRID_W + 1] = { 0 };

	//Get json input
	std::cout << "Importing JSON objects..." << std::endl;
	if (!getObjects(obj_list, "pathin/object_test.json")) {
		std::cout << "[ERROR] File read error" << std::endl;
		return 0;
	}
	else {
		std::cout << "[SUCCESS] JSON objects imported" << std::endl;
	}

	//Fill the terrain grid with objects
	std::cout << std::endl << "Generating terrain grid..." << std::endl;
	makeTerrainGrid(obj_list, terrainGrid);
	std::cout << "[SUCCESS] Terrain grid generated" << std::endl;

	//Generate paths
	std::cout << std::endl << "Generating path from start to cube 0..." << std::endl;
	point<int16_t> start(GRID_H / 2, GRID_W / 2);
	makePath(obj_list, terrainGrid, start, obj_list.obj_cube[0], path);
	if (path.empty()) {
		std::cout << "[ERROR] Generated path returned empty" << std::endl;
	}
	else {
		std::cout << "[SUCCESS] Path generated, length = " << path.size() << " node(s)" << std::endl;
	}

	//Output results
	std::cout << std::endl << "Generating path file..." << std::endl;
	if (!outputPath(path)) {
		std::cout << "[ERROR] Failure generating path file" << std::endl;
	}
	else {
		std::cout << "[SUCCESS] Path file generated" << std::endl;
	}

	#ifdef _DEBUG
	//Test output
	std::cout << std::endl << "Generating terrain debug image..." << std::endl;
	if (!testTerrainGrid(obj_list, terrainGrid, path)) {
		std::cout << "[ERROR] Failed to generate terrain PPM" << std::endl;
	}
	else {
		std::cout << "[SUCCESS] Terrain image generated" << std::endl;
	}
	#endif

	//Cleanup
	return 0;
}
