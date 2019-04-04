#include "json/json.h"
#include <iostream>
#include <fstream>		//std::ifstream, std::ofstream
#include <string>		//std::string
#include <queue>		//std::queue
#include <stack>		//std::stack
#include <vector>		//std::vector
#include <unordered_map>	//std::unordered_map
#include <limits>		//std::numberic_limits
#include <cmath>		//std::floor, std::ceil
#include <cfloat>		//FLT_MAX

/* Const definitions */

//Math expressions
constexpr float PI = 3.1415926f;
constexpr float RT2 = 1.4142135f;

//Field properties
constexpr int CM = 1;
constexpr float GRID = CM / 4.0f;
constexpr int FIELD = 244 * CM;
constexpr int GRID_W = static_cast<const int>(FIELD / GRID);
constexpr int GRID_H = GRID_W;

//Object properties
constexpr int CORNER = 4;
constexpr float obj_cube_w = 3.8f * CM;
constexpr float obj_cube_r = (obj_cube_w * RT2) / 2.0f;
constexpr float obj_obstacle_w = 3.8f * CM;
constexpr float obj_obstacle_r = (obj_obstacle_w * RT2) / 2.0f;
constexpr float obj_mothership_w = 52.5f * CM;
constexpr float obj_mothership_l = 34.29f * CM;
constexpr float obj_mothership_a = (obj_mothership_w * RT2) / 2.0f;
constexpr float obj_mothership_b = (obj_mothership_l * RT2) / 2.0f;
constexpr float obj_robot_w = 21.6f * CM;
constexpr float obj_robot_l = 14.0f * CM;

/* Template Function definitions */

/*
	Object for storing grid coordinates
*/
template <typename T>
struct point {
	T i;
	T j;
	float d;
	point(T I = -1, T J = -1, float D = 0.0f) { 
		this->i = I; 
		this->j = J; 
		this->d = D;
	}

	bool operator==(const point<T> p2) const {
		return (i == p2.i && j == p2.j);
	}

	bool operator!=(const point<T> p2) const {
		return (i != p2.i || j != p2.j);
	}

	bool operator<(const point<T> p2) const {
		return (i < p2.i || (!(p2.i < i) && j < p2.j));
	}
};
//Implement hash function support
namespace std {
	template<typename T>
	struct hash<point<T>> {
		typedef point<T> arg_t;
		typedef std::size_t res_t;
		std::size_t operator()(const point<T> &p) const noexcept {
			return std::hash<T>()(p.i ^ (p.j << 4));
		}
	};
}

/*
	Wrapper for priority_queue that allows you to attatch arbitrary
	elements by a priority value
*/
template<typename T, typename P>
struct PQueue {
	typedef std::pair<P, T> Element;
	std::priority_queue<Element, std::vector<Element>, std::greater<Element>> elements;

	/* Member functions */

	inline bool empty() const {
		return elements.empty();
	}

	inline void put(T item, P priority) {
		elements.emplace(priority, item);
	}

	T get() {
		T ret = elements.top().second;
		elements.pop();
		return ret;
	}
};

/*
	Make a point object inline
*/
template <typename T>
point<T> makePoint(T i, T j, float d = 0.0f) {
	point<T> p(i, j, d);
	return p;
}

/*
	Check if a point is within grid boundaries
*/
template <typename T>
bool pointValid(point<T> p) {
	return  (p.i >= 0) && (p.i < GRID_H) &&
			(p.j >= 0) && (p.j < GRID_W);
}

/*
	Calculate heuristics between two points
*/
template <typename T>
T calculateH(point<T> src, point<T> dest) {
	/*
	//Use a modified Bresenham line algorithm to count integer gridcells from src to dest
	T dj = dest.j - src.j;
	T di = dest.i - src.i;
	T i = src.i;
	T j = src.j;
	T err = 2 * (di - dj);
	T count = 0;
	bool d = false;

	//Calculate for more horizontal slopes
	if (dj >= di) {
		while (j <= dj) {
			if (err >= 0) {
				i++;
				d = true;
				err += (2 * di) - (2 * dj);
			}
			else {
				err += (2 * di);
			}
			count++;
			j++;
		}
	}

	//Calculate for more vertical slopes
	else {
		while (i <= di) {
			if (err >= 0) {
				j++;
				d = true;
				err += (2 * dj) - (2 * di);
			}
			else {
				err += (2 * dj);
			}
			count++;
			i++;
		}
	}

	return count;*/
	/*
	T ci = dest.i - src.i;
	T cj = dest.j - src.j;
	T di = (ci / cj);
	T dj = (cj / ci);
	if (di < 1) {
		di = 1;
	}
	if (dj < 1) {
		dj = 1;
	}
	return (di * ci) + (dj * cj);
	*/
	return abs(dest.j - src.j) + abs(dest.i - src.i);
}

/* 
	Get the minimum of up to 4 values 
*/
template <typename T>
T min(T a, T b, T c = std::numeric_limits<T>::max(), T d = std::numeric_limits<T>::max()) {
	T r = (a < b) ? a : b;
	r = (r < c) ? r : c;
	r = (r < d) ? r : d;
	return r;
}

/*
	Get the maximum of up to 4 values
*/
template <typename T>
T max(T a, T b, T c = std::numeric_limits<T>::min(), T d = std::numeric_limits<T>::min()) {
	T r = (a > b) ? a : b;
	r = (r > c) ? r : c;
	r = (r > d) ? r : d;
	return r;
}

/*
	Contain val between min and max, inclusive
*/
template <typename T>
T bound(T val, T min, T max) {
	if (min >= max) {
		return val;
	}
	else if (val < min) {
		return min;
	}
	else if (val > max) {
		return max;
	}
	else {
		return val;
	}
}

/*
	Since array indexing dictates that y=0 is the top edge and the real world
	coordinates dictate that y=0 is the bottom edge, this function wil
	quickly mirror values across the horizontal middle axis
*/
template <typename T>
T mirrorI(T i) {
	T diff = T(FIELD / 2.0f) - i;
	return T(FIELD / 2.0f) + diff;
}

/* Struct definitions */

struct PROP_MOTHERSHIP {
	float x_pos, y_pos, dir;
	point<float> corner[4] = { -1.0f };

	PROP_MOTHERSHIP() {
		x_pos = y_pos = dir - 1.0f;
	}
};

struct PROP_OBSTACLE {
	float x_pos, y_pos;
	point<float> corner[4] = { -1.0f };

	PROP_OBSTACLE() {
		x_pos = y_pos = -1.0f;
	}
};

struct PROP_CUBE {
	float x_pos, y_pos, dir;
	char letter;
	point<float> corner[4] = { -1.0f };

	PROP_CUBE() {
		x_pos = y_pos = dir = -1.0f;
	}

	bool operator==(const PROP_CUBE c2) const {
		return (x_pos == c2.x_pos && y_pos == c2.y_pos && dir == c2.dir && letter == c2.letter);
	}
};

struct LIST_OBJECTS {
	uint8_t num_obstacle;
	uint8_t num_cube;
	PROP_MOTHERSHIP obj_mothership;
	PROP_OBSTACLE obj_obstacle[15];
	PROP_CUBE obj_cube[6];

	LIST_OBJECTS() {
		num_obstacle = num_cube = 0;
	};
};

/* Function declarations */

bool getObjects(LIST_OBJECTS &L, std::string filename);
void makeTerrainGrid(LIST_OBJECTS &L, uint8_t terrainGrid[][GRID_W + 1]);
void aStar(uint8_t terrainGrid[][GRID_W + 1], point<int16_t> src, point<int16_t> dest, std::vector<point<int16_t>> &path);
void drawFilledCircle(uint8_t terrainGrid[][GRID_W + 1], point<int16_t> c, int16_t r, uint8_t v);
bool testTerrainGrid(LIST_OBJECTS &L, uint8_t terrainGrid[][GRID_W + 1], std::stack<point<int16_t>> &path);
void makePath(LIST_OBJECTS &L, uint8_t terrainGrid[][GRID_W + 1], point<int16_t> src, PROP_CUBE &c, std::stack<point<int16_t>> &path);
void calculateCorners(point<int16_t> p, float w, float l, point<float> (&corners)[4]);
int16_t calculateArea(uint8_t terrainGrid[][GRID_W + 1], point<float> (&corners)[4]);
int16_t calculateSequence(int16_t n);
int16_t mirrorI(int16_t i);
bool outputPath(std::stack<point<int16_t>> &path);
