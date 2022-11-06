#ifndef EULEROPERATIONS
#define EULEROPERATIONS

#include "Brep.h"
//#include <json\json.h>

//using namespace Json;
using namespace std;

class Euler
{
protected:
	Solid* mvfs(glm::vec3 pos);								//make vertex face solid
	HalfEdge* mev(Loop* lp, glm::vec3 oldPos, glm::vec3 newPos);	// make edge vertex
	Loop* mef(Loop* lp, glm::vec3 pos1, glm::vec3 pos2);		// make edge face
	Loop* kemr(Loop* lp, glm::vec3 pos1, glm::vec3 pos2);		// kill edge, make ring
	bool kfmrh(Loop* innerLoop, Loop* outerLoop);				// kill face make ring and hole

	//bool kvfs(Solid* s, double* pcoord);						// kill vertex face solid
	//bool kev(Solid* s, double* coord1, double* coord2);			// kill edge vertex
	//bool kef(Loop* lp, double* coord1, double* coord2);			// kill edge face
	//HalfEdge* mekr(Loop* lp, Loop* todel, double* coord1, double* coord2); //make edge kill ring
	//Face* mfkrh(Loop* outerLoop, Loop* innerLoop);				// make face kill ring and hole

	void Sweep(Face* f, glm::vec3 move_vec);

public:
	static void ListSolid(Solid* s);									// Output Solid Hierarchy For Debug

};



#endif // !EULEROPERATIONS

