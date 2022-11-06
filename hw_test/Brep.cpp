
#include<cstdlib>
#include<cmath>
#include"Brep.h"

// initialize global counter
int Solid::total_solidnum = 0;
int Face::total_facenum = 0;
int Loop::total_loopnum = 0;
int Edge::total_edgenum = 0;
int HalfEdge::total_halfegnum = 0;
int Vertex::total_vertexnum = 0;

const double eps = 1e-5;



/***************************Solid******************************/
/**************************************************************/

// solid constructor
Solid::Solid()
{
	prevs = NULL;
	nexts = NULL;
	sverts = NULL;
	sedges = NULL;
	//sloops = NULL;
	sfaces = NULL;

	numvertex = 0;
	numedge = 0;
	numface = 0;
	//numloop = 0;

	solidno = total_solidnum;
	total_solidnum++;
	cout << "Solid no. " << solidno << " is created. " << endl;

}

// solid destructor
Solid::~Solid(void)
{
	cout << "Solid no. " << solidno << " is deleted. " << endl;
}

// Find Vertex in solid
Vertex* Solid::findVertex(glm::vec3 pos)
{
	Vertex* tmpv = sverts;
	while (tmpv)
	{
		glm::vec3 p = tmpv->pos;
		if (fabs(pos.x-p.x)<0.001f&&fabs(pos.y-p.y)<0.001f&&fabs(pos.z-p.z)<0.001f)
			return tmpv;
		tmpv = tmpv->nextv;
	}

	cout << "Warning: findVertex Falied ! Vertex: ( " << pos.x << "," << pos.y << "," << pos.z << " ) Does not Exist in Solid ." << endl;
	return NULL;
}


bool Solid::deleteVertex(Vertex* v)
{
	if (v == NULL)
	{
		cout << "Error: No Valid Vertex Pointer ! Delete Vertex Failed !" << endl;
		return false;
	}

	// int count = 0;
	Vertex* tmpv = sverts;
	while (tmpv)
	{
		if (tmpv == v)
		{
			if (tmpv->nextv != NULL && tmpv->prevv == NULL) {
				tmpv->nextv->prevv = NULL;
				sverts = tmpv->nextv;
			}
			else if (tmpv->nextv == NULL && tmpv->prevv != NULL)	
			{
				tmpv->prevv->nextv = NULL;
			}
			else if (tmpv->nextv == NULL && tmpv->prevv == NULL)
			{
				this->sverts = NULL;
			}
			else
			{
				tmpv->prevv->nextv = tmpv->nextv;
				tmpv->nextv->prevv = tmpv->prevv;
			}

			numvertex--;
			delete tmpv;
			return true;
		}
		tmpv = tmpv->nextv;
		// count++;
	}

	cout << "Warning: Can Not Find Vertex In Solid ! Delete Vertex Failed !" << endl;
	return false;

}


bool Solid::deleteEdge(Edge* eg)
{
	if (eg == NULL)
	{
		cout << "Error: No Valid Edge Pointer ! Delete Edge Failed !" << endl;
		return false;
	}

	// int count = 0;
	Edge* tmpeg = sedges;
	while (tmpeg)
	{
		if (tmpeg == eg)
		{
			
			if (tmpeg->nexte != NULL && tmpeg->preve == NULL) {
				tmpeg->nexte->preve = NULL;
				sedges = tmpeg->nexte;
			}
			else if (tmpeg->nexte == NULL && tmpeg->preve != NULL)	
			{
				tmpeg->preve->nexte = NULL;
			}
			else if (tmpeg->nexte == NULL && tmpeg->preve == NULL)
			{
				this->sedges = NULL;
			}
			else
			{
				tmpeg->preve->nexte = tmpeg->nexte;
				tmpeg->nexte->preve = tmpeg->preve;

			}

			numedge--;
			delete tmpeg;
			return true;
		}
		tmpeg = tmpeg->nexte;
		// count++;
	}

	cout << "Warning: Can Not Find Edge eg in Solid ! Delete Edge Failed !" << endl;
	return false;
}


bool Solid::deleteFace(Face* face)
{
	if (face == NULL)
	{
		cout << "Error: No Valid Face Pointer ! Delete Face Failed !" << endl;
		return false;
	}

	// int count = 0;
	Face* tmpf = sfaces;
	while (tmpf)
	{
		if (tmpf == face)
		{
			
			if (tmpf->nextf != NULL && tmpf->prevf == NULL) {
				sfaces = tmpf->nextf;
				sfaces->prevf = NULL;
			}
			else if (tmpf->nextf == NULL && tmpf->prevf != NULL)	
			{
				tmpf->prevf->nextf = NULL;
			}
			else if (tmpf->nextf == NULL && tmpf->prevf == NULL)
			{
				this->sfaces = NULL;
			}
			else
			{
				tmpf->prevf->nextf = tmpf->nextf;
				tmpf->nextf->prevf = tmpf->prevf;

			}

			numface--;
			delete tmpf;
			return true;
		}

		tmpf = tmpf->nextf;
		// count++;
	}

	cout << "Warning: Can Not Find Face face in Solid ! Delete Face Failed !" << endl;
	return false;
}



bool Solid::addVertexToSolid(Vertex* v)
{
	if (v == NULL)
	{
		cout << "Error: No Valid Vertex Pointer ! Add Vertex To Solid Failed !" << endl;
		return false;
	}
	if (sverts == NULL)
	{
		sverts = v;
		numvertex++;
		return true;
	}

	Vertex* tmpv = sverts;
	while (tmpv)
	{
		if (tmpv->nextv == NULL)
			break;
		tmpv = tmpv->nextv;
	}

	tmpv->nextv = v;
	v->prevv = tmpv;
	numvertex++;
	return true;
}


bool Solid::addEdgeToSolid(Edge* eg)
{
	if (eg == NULL)
	{
		cout << "Error: No Valid Edge Pointer ! Add Edge To Solid Failed !" << endl;
		return false;
	}

	if (sedges == NULL)
	{
		sedges = eg;
		numedge++;
		return true;
	}

	Edge* tmpeg = sedges;
	while (tmpeg)
	{
		if (tmpeg->nexte == NULL)
			break;
		tmpeg = tmpeg->nexte;
	}

	tmpeg->nexte = eg;
	eg->preve = tmpeg;
	numedge++;
	return true;
}



bool Solid::addFaceToSolid(Face* face)
{
	if (face == NULL)
	{
		cout << "Error: No Valid Face Pointer ! Add Face To Solid Failed !" << endl;
		return false;
	}

	if (sfaces == NULL)
	{
		sfaces = face;
		face->fsolid = this;
		numface++;
		return true;
	}

	Face* tmpf = sfaces;
	while (tmpf)
	{
		if (tmpf->nextf == NULL)
			break;
		tmpf = tmpf->nextf;
	}

	tmpf->nextf = face;
	face->prevf = tmpf;
	face->fsolid = this;
	numface++;
	return true;

}


/***************************Face*******************************/
/**************************************************************/


Face::Face()
{
	fsolid = NULL;
	floops = NULL;
	nextf = NULL;
	prevf = NULL;

	faceno = total_facenum;
	total_facenum++;
	cout << "Face no. " << faceno << " is created. " << endl;
}


Face::~Face()
{
	cout << "Face no. " << faceno << " is deleted. " << endl;
}

int Face::Size()
{
	int cnt = 0;
	Loop* loop = floops;
	while (loop) {
		cnt++;
		loop = loop->nextl;
	}
	return cnt;
}

void Face::setInnerLoop()
{
	int size = Size();
	if (size <= 1) return;

	//计算每个loop中最长半边的长度，将最长半边所在的loop设置为outerloop
	Loop* loop = floops;
	Loop* maxlp = NULL;
	float maxlen = 0;
	while (loop) {
		loop->is_inner_loop = true;
		HalfEdge* he = loop->lhedge;
		glm::vec3 v1, v2;
		float curlen;
		do {
			v1 = he->hestartv->pos;
			v2 = he->next->hestartv->pos;
			curlen = max(curlen, glm::length(v2 - v1));
			he = he->next;
		} while (he != loop->lhedge);

		//如果是第一个环
		if (loop == floops)
		{
			maxlen = curlen;
			maxlp = loop;
		}
		else {	//如果不是第一个环，把大的存在里面
			if (curlen > maxlen) {
				maxlen = curlen;
				maxlp = loop;
			}
		}

		loop = loop->nextl;
	}
	maxlp->is_inner_loop = false;
}

Loop* Face::GetOuterLoop()
{
	for (Loop* loop = floops; loop != NULL; loop = loop->nextl) {
		if (! loop->is_inner_loop)
			return loop;
	}
	return NULL;
}


bool Face::addLoopToFace(Loop* lp)
{
	if (lp == NULL)
	{
		cout << "Error: No Valid Loop Pointer ! Add Loop To Face Failed !" << endl;
		return false;
	}

	if (floops == NULL) floops = lp;
	else
	{
		Loop* tmplp = floops;
		while (tmplp)
		{
			if (tmplp->nextl == NULL)
				break;
			tmplp = tmplp->nextl;
		}

		tmplp->nextl = lp;
		lp->prevl = tmplp;
	}

	lp->lface = this;
	return true;
}

/***************************Loop*******************************/
/**************************************************************/


Loop::Loop()
{
	lface = NULL;
	lhedge = NULL;
	nextl = NULL;
	prevl = NULL;
	is_inner_loop = false;

	loopno = total_loopnum;
	total_loopnum++;
	cout << "Loop no. " << loopno << " is created. " << endl;
}


Loop::~Loop()
{
	cout << "Loop no. " << loopno << " is deleted. " << endl;
}


bool Loop::addHalfEdgeToLoop(HalfEdge* he)
{
	if (he == NULL)
	{
		cout << "No Valid HalfEdge Pointer ! Add HalfEdge To Loop Failed !" << endl;
		return false;
	}

	if (lhedge == NULL) lhedge = he;
	else      
	{
		HalfEdge* tmphe = lhedge;		//尾插
		while (tmphe)
		{
			if (tmphe->next == NULL)
				break;
			tmphe = tmphe->next;
		}
		tmphe->next = he;
		he->prev = tmphe;
	}
	he->wloop = this;
	return true;
}


bool Loop::isVertexInLoop(Vertex* v)
{
	if (v == NULL)
	{
		cout << "Error: No Valid Vertex Poiner, Vertex in Loop Search Failed !" << endl;
		return false;
	}
	HalfEdge* tmphe = lhedge;
	while (tmphe)
	{
		if (tmphe->hestartv == v)
			return true;
		tmphe = tmphe->next;
		if (tmphe == lhedge) break;
	}

	cout << "Warning: Can Not Find Vertex in Loop, Vertex in Loop Search Failed ! " << endl;
	return false;

}


HalfEdge* Loop::findHalfEdge(Vertex* v, HalfEdge* he)
{
	if (v == NULL)
	{
		cout << "Error: No Valid Vertex Pointer, Find HalfEdge in Loop Failed !" << endl;
		return NULL;
	}

	if (lhedge == NULL)
	{
		cout << "No Valid HalfEdge Pointer ! Add HalfEdge To Loop Failed !" << endl;
		return NULL;
	}

	HalfEdge* starthe = lhedge;
	HalfEdge* endhe = lhedge;
	if (he != NULL) starthe = he;

	while (starthe)
	{
		if (starthe->hestartv == v)
			return starthe;
		starthe = starthe->next;
		if (starthe == endhe)	
			break;

	}

	cout << "Warning: Can Not Find HalfEdge in Loop ! Find HalfEdge Failed !" << endl;
	return NULL;
}

/***************************Edge*******************************/
/**************************************************************/

Edge::Edge()
{
	he1 = NULL;
	he2 = NULL;
	nexte = NULL;
	preve = NULL;

	edgeno = total_edgenum;
	total_edgenum++;
	cout << "Edge no. " << edgeno << " is created. " << endl;
}


Edge::~Edge()
{
	cout << "Edge no. " << edgeno << " is deleted. " << endl;
}

/***************************HalfEdge*******************************/
/******************************************************************/


HalfEdge::HalfEdge()
{
	edg = NULL;
	hestartv = NULL;
	wloop = NULL;
	adjacent = NULL;
	next = NULL;
	prev = NULL;

	halfedgeno = total_halfegnum;
	total_halfegnum++;
	cout << "HalfEdge no. " << halfedgeno << " is created. " << endl;
}

HalfEdge::~HalfEdge()
{
	cout << "HalfEdge no. " << halfedgeno << " is deleted. " << endl;
}

/****************************Vertex********************************/
/******************************************************************/


Vertex::Vertex()
{
}

Vertex::Vertex(glm::vec3 pos)
{
	nextv = NULL;
	prevv = NULL;

	vertexno = total_vertexnum;
	total_vertexnum++;

	this->pos = pos;

}


