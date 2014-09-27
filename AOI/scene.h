#ifndef _SCENE_H_
#define _SCENE_H_

#include <map>
#include <list>

#define DISTANCE 5

using namespace std;

class CScene
{
public:
	CScene();
	~CScene();

	void Add(int id, int x, int y, int distance = DISTANCE);
	void Move(int id, int x, int y);
	void Leave(int id);

private:
	struct Object {
		int x;
		int y;
		int id;
		int radius;

		list<Object *>::iterator x_pos;
		list<Object *>::iterator y_pos;

		Object(int _id, int _x, int _y, int _radius) 
			: id(_id)
			, x(_x)
			, y(_y)
			, radius(_radius) {
		}
	};

private:
	typedef map<int, Object *> ObjMap;
	typedef list<Object *> ObjList;

	void Update(Object *object);
	void GetRangeSet(Object *object, ObjMap *set);
	void UpdateObjectPosition(Object *object, int x, int y);

private:
	ObjList obj_x_list_;
	ObjList obj_y_list_;
	ObjMap  obj_set_;

	ObjMap  move_set_;
	ObjMap  enter_set_;
	ObjMap  leave_set_;
};

#endif//_SCENE_H_