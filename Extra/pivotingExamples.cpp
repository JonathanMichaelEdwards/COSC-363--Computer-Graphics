
void pivotShape()
{
	float pointLocal[3] = { -0.15, -1, 0 };
	float pointGlobal[3] = { -0.3, 0, 0 };  // diff between shapes or point
	int height = 0;

	glPushMatrix();
		glColor3f(0, 1, 0);
		// when y is increased
		glTranslatef(pointLocal[0], pointLocal[1] + height, pointLocal[2]); 
		glRotatef(10, 0, 0, 1);
		glTranslatef(-pointLocal[0], -pointLocal[1] - height, -pointLocal[2]); 

		glTranslatef(pointGlobal[0], pointGlobal[1] + height, pointGlobal[2]);  // y - increased

		roofTile();
	glPopMatrix();


	glPushMatrix();
	glColor3f(1, 0, 0);
		glTranslatef(0, height, 0);  // y - increased
		roofTile();
	glPopMatrix();
}


void test(void)
{
// test 1 - passed; P = (-2, -0.3, 0)
// pivoting around a point
	glPushMatrix();
		glColor3f(0, 1, 0);
		// glTranslatef(-1, -0.2, 0);  // pivot position

		// pivot offset to scaleing
		glTranslatef(-2, -0.3, 0);
		glRotatef(90, 0, 0, 1);
		glTranslatef(2, 0.3, 0);

		glTranslatef(-3, -0.2, 0);
		
		glScalef(2, 0.2, 0.4);
		glutSolidCube(1);
	glPopMatrix();

// test object to be rotated around
	glPushMatrix();
		glColor3f(1, 0, 0);
		glScalef(4, 0.6, 0.2);  // test 1
		glutSolidCube(1);
	glPopMatrix();


// // test 2 - passed; P = (-2, -0.3, 0)
// // pivoting around origin
// 	glPushMatrix();
// 		glColor3f(0, 1, 0);
// 		// glTranslatef(-1, -0.2, 0);  // pivot position

// 		// pivoting around position
// 		glTranslatef(-2, -0.3, 0);
// 		glRotatef(0, 0, 0, 1);
// 		glTranslatef(2, 0.3, 0);

// 		glTranslatef(-2, -0.3, 0);  // moveing to offset position between the two shapes
		
// 		glScalef(2, 0.2, 0.4);
// 		glutSolidCube(1);
// 	glPopMatrix();

// // test object to be rotated around
// 	glPushMatrix();
// 		glColor3f(1, 0, 0);
// 		glScalef(4, 0.6, 0.2);  // test 2
// 		glutSolidCube(1);
// 	glPopMatrix();



// // test 3 - passed; P = (-4, -0.3, 0)
// 	glPushMatrix();
// 		glColor3f(0, 1, 0);

// 		glTranslatef(-3, -0.2, 0);    // pivot to actual position

// 		// pivot offset to scaleing
// 		glTranslatef(-1, -0.1, 0.2);
// 		glRotatef(100, 0, 0, 1);
// 		glTranslatef(1, 0.1, -0.2);
		
// 		glScalef(2, 0.2, 0.4);
// 		glutSolidCube(1);
// 	glPopMatrix();

// // test object to be rotated around
// 	glPushMatrix();
// 		glColor3f(1, 0, 0);
// 		glScalef(8, 0.6, 0.4);  // test 3
// 		glutSolidCube(1);
// 	glPopMatrix();
}
