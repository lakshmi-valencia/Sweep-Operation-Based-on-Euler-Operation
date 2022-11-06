#include"Euler.h"
#include<cstdlib>
#include<iostream>
#include<fstream>
#include<sstream>
#include<algorithm>

using namespace std;


/************************************欧拉操作*******************************************/
/**********************************************************************************************/

// 输出solid的所有顶点
void Euler::ListSolid(Solid* s)
{
	Face* f;
	Loop* lp;
	HalfEdge* he;

	cout << endl;
	cout << "**********************************************************************************" << endl;
	cout << "Solid no. " << s->solidno << " Log: " << endl;
	cout << "has " << s->numface << " faces totally" <<endl;
	f = s->sfaces;
	int cnt_f = 0;
	while (f)
	{
		cnt_f++;
		cout << endl;
		cout << endl;
		cout << "------------Face no. " << f->faceno <<"      "<<cnt_f<<" face " << endl;
		f->setInnerLoop();

		lp = f->floops;
		int cnt_lp = 0;
		while (lp)
		{
			cnt_lp++;
			cout << endl;
			cout << "Loop no. " << lp->loopno << " " ;
			if (lp->is_inner_loop) cout << "是内环" << endl;
			else cout << "是外环" << endl;
			he = lp->lhedge;
			do
			{
				cout << "Vertex " << he->hestartv->vertexno << " : ( " << he->hestartv->pos.x << " ," << he->hestartv->pos.y << " ," << he->hestartv->pos.z << " ) .";

			} while ((he = he->next) != lp->lhedge);
			cout << endl;
			lp = lp->nextl;
		}
		cout << "--------------face has " << cnt_lp << " loops" << endl;
		f = f->nextf;
	}
	cout << endl;

}

// MVFS: make vertex face solid
Solid* Euler::mvfs(glm::vec3 pos)
{
	Solid* solid = new Solid();
	Vertex* v = new Vertex(pos);
	Loop* loop = new Loop();
	Face* face = new Face();

	solid->addVertexToSolid(v);
	solid->addFaceToSolid(face);
	face->addLoopToFace(loop);
	// solid->addLoopToSolid(loop);

	return solid;
}

// MEV: make edge vertex
HalfEdge* Euler::mev(Loop* lp, glm::vec3 oldPos, glm::vec3 newPos)
{
	Solid* solid = lp->lface->fsolid;
	HalfEdge* he1, * he2, * tmphe;
	Edge* eg;
	Vertex* vold, * vnew;

	// test whether vertex is in the solid
	vold = solid->findVertex(oldPos);
	if (!vold)
	{
		cout << "Error: MEV: Can Not Find Vertex: (  " << oldPos[0] << " ," << oldPos[1] << " ," << oldPos[2] << " ) in Solid no " << solid->solidno << endl;
		return NULL;
	}

	he1 = new HalfEdge();
	he2 = new HalfEdge();
	eg = new Edge();
	vnew = new Vertex(newPos);

	eg->he1 = he1;
	eg->he2 = he2;
	he1->edg = he2->edg = eg;

	he1->hestartv = vold;
	he1->adjacent = he2;

	he2->hestartv = vnew;
	he2->adjacent = he1;

	he1->next = he2;
	he2->prev = he1;
	he1->wloop = he2->wloop = lp;

	if (lp->lhedge == NULL)		//lp中本来没有半边
	{
		he2->next = he1;
		he1->prev = he2;
		lp->lhedge = he1;
	}
	else
	{
		//lp中本来有半边，找到以vold为终点的半边,插入
		tmphe = lp->lhedge;
		do {
			glm::vec3 p = tmphe->hestartv->pos;
			cout << "   p = ( " << p.x << " , " << p.y << " , " << p.z << " )" << endl;
			tmphe = tmphe->next;
		} while (tmphe != lp->lhedge);

		for (tmphe = lp->lhedge; tmphe->next->hestartv != vold; tmphe = tmphe->next);

		he2->next = tmphe->next;
		tmphe->next->prev = he2;

		tmphe->next = he1;
		he1->prev = tmphe;
	}
	he2->wloop = lp;
	he1->wloop = lp;
	solid->addVertexToSolid(vnew);
	solid->addEdgeToSolid(eg);

	return he1;

}

// 在两个给定点之间生成一个新边，同时生成一个新环，一个新面
Loop* Euler::mef(Loop* lp, glm::vec3 pos1, glm::vec3 pos2)
{
	Edge* nedge = new Edge();
	Face* nface = new Face();
	Loop* nloop = new Loop();
	Solid* s = lp->lface->fsolid;

	// 找到两个位置对应的点
	Vertex* v1 = s->findVertex(pos1);
	Vertex* v2 = s->findVertex(pos2);

	if (!v1)
	{
		cout << "Error: MEF: Can Not Find Vertex: (  " << pos1.x << " ," << pos1.y << " ," << pos1.z << " ) in Solid no " << s->solidno << endl;
		return NULL;
	}

	if (!v2)
	{
		cout << "Error: MEF: Can Not Find Vertex: (  " << pos2.x << " ," << pos2.y << " ," << pos2.z << " ) in Solid no " << s->solidno << endl;
		return NULL;
	}

	// 判断两点是否在loop中
	HalfEdge* starthe = lp->findHalfEdge(v1);
	HalfEdge* endhe = lp->findHalfEdge(v2);

	if (starthe == NULL)
	{
		cout << "Error: MEF: Can Not Find Vertex: (  " << pos1.x << " ," << pos1.y << " ," << pos1.z << " ) in Loop no " << lp->loopno << endl;
		return NULL;
	}

	if (endhe == NULL)
	{
		cout << "Error: MEF: Can Not Find Vertex: (  " << pos2.x << " ," << pos2.y << " ," << pos2.z << " ) in Loop no " << lp->loopno << endl;
		return NULL;
	}

	HalfEdge* he1 = new HalfEdge();
	HalfEdge* he2 = new HalfEdge();
	HalfEdge* tmphe;

	nloop->addHalfEdgeToLoop(he2);
	
	tmphe = starthe;
	while (tmphe)
	{
		if (tmphe == endhe)
			break;
		tmphe->wloop = nloop;
		tmphe = tmphe->next;
	}
	he2->next = starthe;
	tmphe = starthe->prev;
	starthe->prev = he2;

	endhe->prev->next = he2;
	he2->prev = endhe->prev;

	tmphe->next = he1;
	he1->prev = tmphe;

	he1->next = endhe;
	endhe->prev = he1;

	he1->wloop = lp;
	lp->lhedge = he1;


	he1->adjacent = he2;
	he2->adjacent = he1;
	he1->edg = nedge;
	he2->edg = nedge;

	nedge->he1 = he1;
	nedge->he2 = he2;
	he1->hestartv = v1;
	he2->hestartv = v2;

	nface->addLoopToFace(nloop);
	s->addEdgeToSolid(nedge);
	s->addFaceToSolid(nface);
	// s->addLoopToSolid(nloop);

	return nloop;
}

//删去一条物理边，生成一个环
Loop* Euler::kemr(Loop* lp, glm::vec3 pos1, glm::vec3 pos2)
{
	Solid* s = lp->lface->fsolid;
	
	Vertex* v1 = s->findVertex(pos1);
	Vertex* v2 = s->findVertex(pos2);

	if (!v1)
	{
		cout << "Error: KEMR: Can Not Find Vertex: (  " << pos1.x << " ," << pos1.y << " ," << pos1.z << " ) in Solid no " << s->solidno << endl;
		return NULL;
	}

	if (!v2)
	{
		cout << "Error: KEMR: Can Not Find Vertex: (  " << pos2.x << " ," << pos2.y << " ," << pos2.z << " ) in Solid no " << s->solidno << endl;
		return NULL;
	}


	if (!lp->isVertexInLoop(v1))
	{
		cout << "Error: MEF: Can Not Find Vertex: (  " << pos1.x << " ," << pos1.y << " ," << pos1.z << " ) in Loop no " << lp->loopno << endl;
		return NULL;
	}

	if (!lp->isVertexInLoop(v2))
	{
		cout << "Error: MEF: Can Not Find Vertex: (  " << v2->pos.x << " ," << v2->pos.y << " ," << v2->pos.z << " ) in Loop no " << lp->loopno << endl;
		cout << v2->vertexno << endl;
		HalfEdge* tmphe = lp->lhedge;
		do {
			glm::vec3 p = tmphe->hestartv->pos;
			cout <<"no."<<tmphe->hestartv->vertexno << "   p = ( " << p.x << " , " << p.y << " , " << p.z << " )" << endl;
			tmphe = tmphe->next;
		} while (tmphe != lp->lhedge);

		return NULL;
	}

	// 找到分别以v1,v2为起点的半边he1,he2
	HalfEdge* he1 = lp->lhedge;
	while (he1)
	{
		if (he1->hestartv == v1 && he1->next->hestartv == v2)
			break;
		he1 = he1->next;
	}
	HalfEdge* he2 = he1->adjacent;
	Loop* nloop = new Loop();

	if (he1->next != he2)
	{
		nloop->addHalfEdgeToLoop(he1->next);
		HalfEdge* tmphe = he1->next->next;
		while (tmphe)
		{
			if (tmphe == he2)
				break;
			tmphe->wloop = nloop;
			tmphe = tmphe->next;
		}

		
		he1->next->prev = he2->prev;
		he2->prev->next = he1->next;
	}

	lp->lface->addLoopToFace(nloop);
	
	lp->lhedge = he1->prev;

	
	he1->prev->next = he2->next;
	he2->next->prev = he1->prev;

	s->deleteEdge(he1->edg);
	delete he1;
	delete he2;

	return nloop;

}

//删去一个面，把这个面的环变成另一个面的内环
bool Euler::kfmrh(Loop* innerLoop, Loop* outerLoop)
{
	if (innerLoop == NULL)
	{
		cout << "Error: KFMRH: No Valid innerLoop Pointer !" << endl;
		return false;
	}

	if (outerLoop == NULL)
	{
		cout << "Error: KFMRH: No Valid outerLoop Pointer !" << endl;
		return false;
	}

	// innerloop lface
	Face* tmpf = innerLoop->lface;
	innerLoop->is_inner_loop = true;
	outerLoop->lface->addLoopToFace(innerLoop);
	// find the innerloop face in face list and delete
	if (tmpf->prevf == NULL && tmpf->nextf == NULL)
	{
		cout << "Error: KFMRH: Inner Loop Face is not closed !" << endl;
		return false;
	}
	else
	{
		innerLoop->lface->fsolid->deleteFace(tmpf);
		return true;
	}
}


void Euler::Sweep(Face* f, glm::vec3 move_vec)
{
	glm::vec3 p1, p2;
	glm::vec3 firstp, lastp;

	Vertex* startv;
	HalfEdge* tmphe, * nextp, * prevp;
	for (Loop* tmplp = f->floops; tmplp != NULL; tmplp = tmplp->nextl)	//遍历所有环
	{
		
		tmphe = tmplp->lhedge;
		nextp = tmphe->next;
		prevp = tmphe->prev;

		startv = tmphe->hestartv;
		p1 = startv->pos;

		p2 = p1 + move_vec;

		firstp = lastp = p2;
		// make first edge
		mev(tmplp, p1, p2);

		if (tmphe == prevp)
			tmphe = NULL;
		else
		{
			tmphe = nextp;
			nextp = tmphe->next;
		}

		while (tmphe)
		{
			startv = tmphe->hestartv;
			p1 = startv->pos;
			p2 = p1 + move_vec;

			mev(tmplp, p1, p2);
			mef(tmplp, lastp, p2);
			lastp = p2;

			//如果到达最后一条半边
			if (tmphe == prevp)
			{
				mef(tmplp, lastp, firstp);
				tmphe = NULL;
			}
			else
			{
				tmphe = nextp;
				nextp = tmphe->next;
			}

		}

	}
}




//
//// KVFS: kill vertex Face Solid
//bool EulerOperations::kvfs(Solid* s, double* pcoord)
//{
//	Vertex* v = s->findVertex(pcoord);
//	if (v == NULL)
//	{
//		cout << "Error: KVFS: Can Not Find Vertex: (  " << pcoord[0] << " ," << pcoord[1] << " ," << pcoord[2] << " ) in Solid no " << s->solidno << endl;
//		return false;
//	}
//
//	Face* face = s->sfaces;
//	Loop* lp = face->floops;
//	delete v;
//	delete lp;
//	delete face;
//	delete s;
//
//	return true;
//}
//
//// KER: kill edge vertex
//bool EulerOperations::kev(Solid* s, double* coord1, double* coord2)
//{
//	Vertex* v1 = s->findVertex(coord1);
//	Vertex* v2 = s->findVertex(coord2);
//
//	if (v1 == NULL)
//	{
//		cout << "Error: KER: Can Not Find Vertex: (  " << coord1[0] << " ," << coord1[1] << " ," << coord1[2] << " ) in Solid no " << s->solidno << endl;
//		return false;
//	}
//
//	if (v2 == NULL)
//	{
//		cout << "Error: KER: Can Not Find Vertex: (  " << coord2[0] << " ," << coord2[1] << " ," << coord2[2] << " ) in Solid no " << s->solidno << endl;
//		return false;
//	}
//
//	// find edge contains v1 and v2
//	Edge* tmpedg = s->sedges;
//	while (tmpedg)
//	{
//		if ((tmpedg->he1->hev == v1 && tmpedg->he2->hev == v2)
//			|| (tmpedg->he1->hev == v2 && tmpedg->he2->hev == v1))
//			return tmpedg;
//		tmpedg = tmpedg->nexte;
//	}
//
//	if (tmpedg == NULL)
//	{
//		cout << "Error: KER: Can Not Find Edge Bridge V1 and V2 !" << endl;
//		return false;
//	}
//
//	Edge* edge2del = tmpedg;
//	HalfEdge* dehe1 = edge2del->he1;
//	HalfEdge* dehe2 = edge2del->he2;
//
//	Vertex* delv2 = v2;
//	// adjust points orientation
//	Face* face = s->sfaces;
//	while (face)
//	{
//		Loop* tmplp = face->floops;
//		while (tmplp)
//		{
//			HalfEdge* tmphe = tmplp->findHalfEdge(delv2);
//			if (tmphe != NULL)
//				tmphe->hev = v1;
//			tmplp = tmplp->nextl;
//		}
//		face = face->nextf;
//	}
//
//	// delete two HalfEdges with the Edge
//	dehe1->wloop->ledge = dehe1->nxt;
//	dehe2->wloop->ledge = dehe2->nxt;
//	dehe1->nxt->prv = dehe1->prv;
//	dehe1->prv->nxt = dehe1->nxt;
//	dehe2->nxt->prv = dehe2->prv;
//	dehe2->prv->nxt = dehe2->nxt;
//
//	delete dehe1;
//	delete dehe2;
//
//	// delete Edge in solid
//	s->deleteEdge(edge2del);
//	// delete vertex 2
//	s->deleteVertex(delv2);
//
//	return true;
//}
//
//// KEF: kill edge face
//bool EulerOperations::kef(Loop* lp, double* coord1, double* coord2)
//{
//	Solid* s = lp->lface->fsolid;
//
//	Vertex* v1 = s->findVertex(coord1);
//	Vertex* v2 = s->findVertex(coord2);
//
//	// Judege two points are in the solid
//	if (v1 == NULL)
//	{
//		cout << "Error: KEF: Can Not Find Vertex: (  " << coord1[0] << " ," << coord1[1] << " ," << coord1[2] << " ) in Solid no " << s->solidno << endl;
//		return false;
//	}
//
//	if (v2 == NULL)
//	{
//		cout << "Error: KEF: Can Not Find Vertex: (  " << coord2[0] << " ," << coord2[1] << " ," << coord2[2] << " ) in Solid no " << s->solidno << endl;
//		return false;
//	}
//
//	// Judege two points are in the loop
//	if (!lp->isVertexInLoop(v1))
//	{
//		cout << "Error: KEF: Can Not Find Vertex: (  " << coord1[0] << " ," << coord1[1] << " ," << coord1[2] << " ) in Loop no " << lp->loopno << endl;
//		return false;
//	}
//
//	if (!lp->isVertexInLoop(v2))
//	{
//		cout << "Error: KEF: Can Not Find Vertex: (  " << coord1[0] << " ," << coord1[1] << " ," << coord1[2] << " ) in Loop no " << lp->loopno << endl;
//		return false;
//	}
//
//	HalfEdge* delhe1 = lp->findHalfEdge(v1);
//	HalfEdge* delhe2 = delhe1->brother;
//	Edge* delEdge = delhe1->edg;
//	Loop* delLoop = delhe1->wloop;
//	Face* delFace = delLoop->lface;
//
//	HalfEdge* tmphe = delhe2->nxt;
//
//	while (tmphe)
//	{
//		if (tmphe == delhe2)
//			break;
//		tmphe->wloop = lp;
//		tmphe = tmphe->nxt;
//	}
//
//	delhe1->nxt->prv = delhe2->prv;
//	delhe2->prv->nxt = delhe1->nxt;
//	delhe1->prv->nxt = delhe2->nxt;
//	delhe2->nxt->prv = delhe1->prv;
//	lp->ledge = delhe1->nxt;
//
//	// delete halfedges
//	delete delhe1;
//	delete delhe2;
//
//	// delete edge, loop ,face
//	s->deleteEdge(delEdge);
//	s->deleteLoop(delLoop);
//	s->deleteFace(delFace);
//	return true;
//
//}
//
//// MEKR: make edge kill ring
//HalfEdge* EulerOperations::mekr(Loop* lp, Loop* todelLp, double* coord1, double* coord2)
//{
//	Solid* s = lp->lface->fsolid;
//
//	Vertex* v1 = s->findVertex(coord1);
//	Vertex* v2 = s->findVertex(coord2);
//
//	// Judege two points are in the solid
//	if (v1 == NULL)
//	{
//		cout << "Error: MEKR: Can Not Find Vertex: (  " << coord1[0] << " ," << coord1[1] << " ," << coord1[2] << " ) in Solid no " << s->solidno << endl;
//		return NULL;
//	}
//
//	if (v2 == NULL)
//	{
//		cout << "Error: MEKR: Can Not Find Vertex: (  " << coord2[0] << " ," << coord2[1] << " ," << coord2[2] << " ) in Solid no " << s->solidno << endl;
//		return NULL;
//	}
//
//	// Judege two points are in the loop
//	if (!lp->isVertexInLoop(v1))
//	{
//		cout << "Error: MEKR: Can Not Find Vertex: (  " << coord1[0] << " ," << coord1[1] << " ," << coord1[2] << " ) in Loop no " << lp->loopno << endl;
//		return NULL;
//	}
//
//	if (!todelLp->isVertexInLoop(v2))
//	{
//		cout << "Error: MEKR: Can Not Find Vertex: (  " << coord1[0] << " ," << coord1[1] << " ," << coord1[2] << " ) in Loop no " << lp->loopno << endl;
//		return NULL;
//	}
//
//	HalfEdge* he1 = new HalfEdge();
//	HalfEdge* he2 = new HalfEdge();
//	Edge* nEdge = new Edge();
//
//	he1->hev = v1;
//	he2->hev = v2;
//	he1->brother = he2;
//	he2->brother = he1;
//	he1->edg = nEdge;
//	he2->edg = nEdge;
//	nEdge->he1 = he1;
//	nEdge->he2 = he2;
//	he1->wloop = lp;
//	he2->wloop = lp;
//
//	HalfEdge* v2he = todelLp->findHalfEdge(v2);
//	HalfEdge* tmphe = v2he;
//	while (tmphe)
//	{
//		tmphe->wloop = todelLp;
//		tmphe = tmphe->nxt;
//		if (tmphe == v2he)
//			break;
//	}
//
//	HalfEdge* v1he = lp->findHalfEdge(v1);
//	v1he->prv->nxt = he1;
//	he1->prv = v1he->prv;
//	he1->nxt = v2he;
//
//	tmphe = v2he->prv;
//	v2he->prv = he1;
//	tmphe->nxt = he2;
//	he2->prv = tmphe;
//	he2->nxt = v1he;
//	v1he->prv = he2;
//
//	s->addEdgeToSolid(nEdge);
//	s->deleteLoop(todelLp);
//
//	return he1;
//
//}
//
//// MFKRH: make face kill ring and hole
//Face* EulerOperations::mfkrh(Loop* outerLoop, Loop* innerLoop)
//{
//	if (!outerLoop)
//	{
//		cout << "Error: MFKRH: No Valid Outer Loop Pointer !" << endl;
//		return NULL;
//	}
//
//	if (!innerLoop)
//	{
//		cout << "Error: MFKRH: No Valid Inner Loop Poiner !" << endl;
//		return NULL;
//	}
//
//	if (outerLoop->lface != innerLoop->lface)
//	{
//		cout << "Error: MFKRH: The Face Of Outer Loop and Inner Loop Is Not The Same !" << endl;
//		return NULL;
//	}
//
//	// make the innerloop became a face
//	// first loop
//	if (innerLoop->nextl != NULL && innerLoop->prevl == NULL)
//	{
//		innerLoop->nextl->prevl = NULL;
//		innerLoop->lface->fsolid->sfaces->floops = innerLoop->nextl;
//	}
//	// last loop
//	else if (innerLoop->nextl == NULL && innerLoop->prevl != NULL)
//	{
//		innerLoop->prevl->nextl = NULL;
//	}
//	else if (innerLoop->nextl == NULL && innerLoop->prevl == NULL)
//	{
//		innerLoop->lface->fsolid->sfaces->floops = NULL;
//	}
//	else
//	{
//		innerLoop->nextl->prevl = innerLoop->prevl;
//		innerLoop->prevl->nextl = innerLoop->nextl;
//	}
//
//	Face* nFace = new Face();
//	outerLoop->lface->fsolid->addFaceToSolid(nFace);
//	nFace->addLoopToFace(innerLoop);
//
//	return nFace;
//}
