#include"SolidProcess.h"
// Test Cases
Solid* SolidProcess::SolidMultiRing()
{
	glm::vec3 points_up_out[] = 
	{   
		glm::vec3( -0.6, -0.8, 0.0 ),
		glm::vec3(0.6, -0.8, 0.0 ),
		glm::vec3( 0.6, 0.8, 0.0 ),
		glm::vec3( -0.6, 0.8, 0.0 )
	};
	glm::vec3 points_up_in_1[] = 
	{
		glm::vec3( -0.3, 0.1, 0.0 ),
		glm::vec3( 0.3, 0.1, 0.0 ),
		glm::vec3( 0.3, 0.7, 0.0 ),
		glm::vec3( -0.3, 0.7, 0.0 ) 
	};

	glm::vec3 points_up_in_2[] = 
	{ 
		glm::vec3(-0.3,-0.1,0.0),
		glm::vec3(0.3,-0.1,0.0),
		glm::vec3(0.3,-0.7,0.0),
		glm::vec3(-0.3,-0.7,0.0)
	};
	

	// up
	Solid* solid_up = mvfs(points_up_out[0]);
	Loop* assistLp = solid_up->sfaces->floops;
	Loop* outloop = assistLp;

	// out frame
	mev(assistLp, points_up_out[0], points_up_out[1]);
	mev(assistLp, points_up_out[1], points_up_out[2]);
	mev(assistLp, points_up_out[2], points_up_out[3]);
	mef(assistLp, points_up_out[3], points_up_out[0]);

	// inside
	assistLp = solid_up->sfaces->nextf->floops;
	Loop* myLp = assistLp;
	mev(assistLp, points_up_out[0], points_up_in_1[0]);
	assistLp = kemr(assistLp, points_up_out[0], points_up_in_1[0]);
	mev(assistLp, points_up_in_1[0], points_up_in_1[1]);
	mev(assistLp, points_up_in_1[1], points_up_in_1[2]);
	mev(assistLp, points_up_in_1[2], points_up_in_1[3]);
	Loop* inloop = mef(assistLp, points_up_in_1[3], points_up_in_1[0]);
	kfmrh(inloop, outloop);

	//inside2
	assistLp = myLp;
	mev(assistLp, points_up_out[0], points_up_in_2[0]);
	assistLp = kemr(assistLp, points_up_out[0], points_up_in_2[0]);
	mev(assistLp, points_up_in_2[0], points_up_in_2[1]);
	mev(assistLp, points_up_in_2[1], points_up_in_2[2]);
	mev(assistLp, points_up_in_2[2], points_up_in_2[3]);
	inloop = mef(assistLp, points_up_in_2[3], points_up_in_2[0]);
	kfmrh(inloop, outloop);


	ListSolid(solid_up);

	glm::vec3 move_vec = glm::vec3(0.0f, 0.0f, 0.5f);

	//solid_up->sfaces->nextf = NULL;	//debug：这里不应该直接等于null，但是无法
	ListSolid(solid_up);
	Sweep(solid_up->sfaces, move_vec);

	return solid_up;
}

