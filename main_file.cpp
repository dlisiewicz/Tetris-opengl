#define GLM_FORCE_RADIANS

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdlib.h>
#include <stdio.h>
#include "constants.h"
#include "allmodels.h"
#include "lodepng.h"
#include "shaderprogram.h"
#include "myCube.h"
#include <math.h>
#include <iostream>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

float objX = 0.0f;
float objY = 0.0f;
float objZ = 0.0f;
float aspectRatio = 1;

int gameTable[10][10][15];
int currentObject[10][10][15];
int currentObject_cp[10][10][15];
int shadow[10][10][15];

int tetromino1[10][10][15];
int tetromino2[10][10][15];
int tetromino3[10][10][15];
int tetromino4[10][10][15];
int tetromino5[10][10][15];

ShaderProgram* sp;

float* vertices = myCubeVertices;
float* normals = myCubeNormals;
float* texCoords = myCubeTexCoords;
float* colors = myCubeColors;
int vertexCount = myCubeVertexCount;

bool con = false;
int id = 0;
int tetrominoId = 1;

GLuint tex1, tex2, tex3, tex4, tex5, tex_floor, tex_shadow, tex_letters;

std::vector<glm::vec4> verts;
std::vector<glm::vec4> norms;
std::vector<glm::vec2> texCoords1;
std::vector<unsigned int> Indices;

std::vector<glm::vec4> verts_N;
std::vector<glm::vec4> norms_N;
std::vector<glm::vec2> texCoords1_N;
std::vector<unsigned int> Indices_N;

std::vector<glm::vec4> verts_E;
std::vector<glm::vec4> norms_E;
std::vector<glm::vec2> texCoords1_E;
std::vector<unsigned int> Indices_E;

std::vector<glm::vec4> verts_X;
std::vector<glm::vec4> norms_X;
std::vector<glm::vec2> texCoords1_X;
std::vector<unsigned int> Indices_X;

std::vector<glm::vec4> verts_T;
std::vector<glm::vec4> norms_T;
std::vector<glm::vec2> texCoords1_T;
std::vector<unsigned int> Indices_T;

void error_callback(int error, const char* description) {
	fputs(description, stderr);
}

void moveRight() {
	bool canMove = true;


	for (int j = 0; j < 10; j++)
	{
		for (int k = 0; k < 15; k++)
		{
			if (currentObject[9][j][k] != 0)
				canMove = false;
		}
	}
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			for (int k = 1; k < 15; k++)
			{
				if (currentObject[i][j][k] != 0 && gameTable[i][j][k] != 0 &&
					currentObject[i+1][j][k] == 0 && gameTable[i+1][j][k] != 0)
				{
					canMove = false;
				}
			}
		}
	}

	if (canMove)
	{
		for (int i = 0; i < 10; i++)
		{
			for (int j = 0; j < 10; j++)
			{
				for (int k = 0; k < 15; k++)
				{
					currentObject_cp[i][j][k] = currentObject[i][j][k];
				}
			}
		}
		for (int i = 0; i < 10; i++)
		{
			for (int j = 0; j < 10; j++)
			{
				for (int k = 0; k < 15; k++)
				{
					currentObject[i][j][k] = currentObject_cp[i-1][j][k];
					if (currentObject[i][j][k] != 0)
					{
						gameTable[i][j][k] = currentObject[i][j][k];
						if(currentObject_cp[i - 2][j][k]==0) gameTable[i-1][j][k] = 0;
					}
				}
			}
		}
		
	}
	
}
void moveLeft() {
	bool canMove = true;


	for (int j = 0; j < 10; j++)
	{
		for (int k = 0; k < 15; k++)
		{
			if (currentObject[0][j][k] != 0)
				canMove = false;
		}
	}
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			for (int k = 1; k < 15; k++)
			{
				if (currentObject[i][j][k] != 0 && gameTable[i][j][k] != 0 &&
					currentObject[i - 1][j][k] == 0 && gameTable[i - 1][j][k] != 0)
				{
					canMove = false;
				}
			}
		}
	}

	if (canMove)
	{
		for (int i = 0; i < 10; i++)
		{
			for (int j = 0; j < 10; j++)
			{
				for (int k = 0; k < 15; k++)
				{
					currentObject_cp[i][j][k] = currentObject[i][j][k];
				}
			}
		}
		for (int i = 0; i < 10; i++)
		{
			for (int j = 0; j < 10; j++)
			{
				for (int k = 0; k < 15; k++)
				{
					currentObject[i][j][k] = currentObject_cp[i + 1][j][k];
					if (currentObject[i][j][k] != 0)
					{
						gameTable[i][j][k] = currentObject[i][j][k];
						gameTable[i + 1][j][k] = 0;
					}
				}
			}
		}

	}
}
void moveUp() {
	bool canMove = true;


	for (int j = 0; j < 10; j++)
	{
		for (int k = 0; k < 15; k++)
		{
			if (currentObject[j][9][k] != 0)
				canMove = false;
		}
	}
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			for (int k = 1; k < 15; k++)
			{
				if (currentObject[i][j][k] != 0 && gameTable[i][j][k] != 0 &&
					currentObject[i][j+1][k] == 0 && gameTable[i][j+1][k] != 0)
				{
					canMove = false;
				}
			}
		}
	}
	if (canMove)
	{
		for (int i = 0; i < 10; i++)
		{
			for (int j = 0; j < 10; j++)
			{
				for (int k = 0; k < 15; k++)
				{
					currentObject_cp[i][j][k] = currentObject[i][j][k];
				}
			}
		}
		for (int i = 0; i < 10; i++)
		{
			for (int j = 0; j < 10; j++)
			{
				for (int k = 0; k < 15; k++)
				{
					currentObject[i][j][k] = currentObject_cp[i][j - 1][k];
					if (currentObject[i][j][k] != 0)
					{
						gameTable[i][j][k] = currentObject[i][j][k];
						if (currentObject_cp[i][j-2][k] == 0) gameTable[i][j-1][k] = 0;
					}
				}
			}
		}
	}
}
void moveDown() {
	bool canMove = true;


	for (int j = 0; j < 10; j++)
	{
		for (int k = 0; k < 15; k++)
		{
			if (currentObject[j][0][k] != 0)
				canMove = false;
		}
	}
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			for (int k = 1; k < 15; k++)
			{
				if (currentObject[i][j][k] != 0 && gameTable[i][j][k] != 0 &&
					currentObject[i][j - 1][k] == 0 && gameTable[i][j - 1][k] != 0)
				{
					canMove = false;
				}
			}
		}
	}
	if (canMove)
	{
		for (int i = 0; i < 10; i++)
		{
			for (int j = 0; j < 10; j++)
			{
				for (int k = 0; k < 15; k++)
				{
					currentObject_cp[i][j][k] = currentObject[i][j][k];
				}
			}
		}
		for (int i = 0; i < 10; i++)
		{
			for (int j = 0; j < 10; j++)
			{
				for (int k = 0; k < 15; k++)
				{
					currentObject[i][j][k] = currentObject_cp[i][j + 1][k];
					if (currentObject[i][j][k] != 0)
					{
						gameTable[i][j][k] = currentObject[i][j][k];
						gameTable[i][j + 1][k] = 0;
					}
				}
			}
		}
	}
}

void moveToFloor() {
	int height = 15;
	int minHeight = 15;
	bool gotMinObj = false;
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			for (int k = 0; k < 15; k++)
			{
				if (gameTable[i][j][k] != 0 && currentObject[i][j][k] == 0) height = 14 - k;
				if (currentObject[i][j][k] != 0 && gotMinObj == false) {
					
					height = height - (15 - k);
					gotMinObj = true;
					if (height < minHeight) {
						minHeight = height;
					}
				}
			}
			height = 15;
			gotMinObj = false;
		}
	}

	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			for (int k = 0; k < 15; k++)
			{
				if (gameTable[i][j][k - minHeight] == 0 && currentObject[i][j][k] != 0 
					&& k>= minHeight)
				{
					gameTable[i][j][k - minHeight] = currentObject[i][j][k];
					gameTable[i][j][k] = 0;
				}
			}
		}
	}

	con = false;
}

//Procedura obsługi klawiatury
void key_callback(GLFWwindow* window, int key,
	int scancode, int action, int mods) {

	if (action == GLFW_PRESS) {
		if (key == GLFW_KEY_LEFT) {
			moveLeft();
		}
		if (key == GLFW_KEY_RIGHT) {
			moveRight();
		}
		if (key == GLFW_KEY_DOWN) {
			moveDown();
		}
		if (key == GLFW_KEY_UP) {
			moveUp();
		}
		if (key == GLFW_KEY_SPACE) {
			moveToFloor();
		}
	}

	if (action == GLFW_RELEASE) {
	}

}

GLuint readTexture(const char* filename) {
	GLuint tex;
	glActiveTexture(GL_TEXTURE0);

	//Wczytanie do pamięci komputera
	std::vector<unsigned char> image;   //Alokuj wektor do wczytania obrazka
	unsigned width, height;   //Zmienne do których wczytamy wymiary obrazka
	//Wczytaj obrazek
	unsigned error = lodepng::decode(image, width, height, filename);

	//Import do pamięci karty graficznej
	glGenTextures(1, &tex); //Zainicjuj jeden uchwyt
	glBindTexture(GL_TEXTURE_2D, tex); //Uaktywnij uchwyt
	//Wczytaj obrazek do pamięci KG skojarzonej z uchwytem
	glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, (unsigned char*)image.data());

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return tex;
}

void windowResizeCallback(GLFWwindow* window, int width, int height) {
	if (height == 0) return;
	aspectRatio = (float)width / (float)height;
	glViewport(0, 0, width, height);
}

void loadModel(std::string plik, int id) {
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(plik, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals);
	std::cout << importer.GetErrorString() << std::endl;

	aiMesh* mesh = scene->mMeshes[id];

	for (int i = 0; i < mesh->mNumVertices; i++)
	{
		aiVector3D vertex = mesh->mVertices[i];
		verts.push_back(glm::vec4(vertex.x,vertex.y,vertex.z, 1));

		aiVector3D normal = mesh->mNormals[i];
		norms.push_back(glm::vec4(normal.x, normal.y, normal.z, 0));

		aiVector3D texCoord = mesh->mTextureCoords[0][i];
		texCoords1.push_back(glm::vec2(texCoord.x,texCoord.y));
	}

	for (int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace& face = mesh->mFaces[i];

		for (int j = 0; j < face.mNumIndices; j++)
		{
			Indices.push_back(face.mIndices[j]);
		}
	}

}
//Procedura inicjująca
void initOpenGLProgram(GLFWwindow* window) {
	//************Tutaj umieszczaj kod, który należy wykonać raz, na początku programu************
	glClearColor(0, 0.4, 0.4, 1);//Ustaw czarny kolor czyszczenia ekranu
	glEnable(GL_DEPTH_TEST); //Włącz test głębokości pikseli
	glfwSetWindowSizeCallback(window, windowResizeCallback);
	glfwSetKeyCallback(window, key_callback); //Zarejestruj procedurę obsługi klawiatury

	sp = new ShaderProgram("v_simplest.glsl", NULL, "f_simplest.glsl");
	tex_floor = readTexture("res/stone.png");
	tex_shadow = readTexture("res/stone1.png");
	tex1 = readTexture("res/diamond.png");
	tex2 = readTexture("res/emerald.png");
	tex3 = readTexture("res/iron.png");
	tex4 = readTexture("res/wood.png");
	tex5 = readTexture("res/sand.png");
	tex_letters = readTexture("res/wood.png");


	loadModel(std::string("obj/Letters.fbx"), 34);

	verts_N = verts;
	norms_N = norms;
	texCoords1_N = texCoords1;
	Indices_N = Indices;

	verts.clear();
	norms.clear();
	texCoords1.clear();
	Indices.clear();

	loadModel(std::string("obj/Letters.fbx"), 43);

	verts_E = verts;
	norms_E = norms;
	texCoords1_E = texCoords1;
	Indices_E = Indices;

	verts.clear();
	norms.clear();
	texCoords1.clear();
	Indices.clear();

	loadModel(std::string("obj/Letters.fbx"), 24);

	verts_X = verts;
	norms_X = norms;
	texCoords1_X = texCoords1;
	Indices_X = Indices;

	verts.clear();
	norms.clear();
	texCoords1.clear();
	Indices.clear();

	loadModel(std::string("obj/Letters.fbx"), 28);

	verts_T = verts;
	norms_T = norms;
	texCoords1_T = texCoords1;
	Indices_T = Indices;
}


//Zwolnienie zasobów zajętych przez program
void freeOpenGLProgram(GLFWwindow* window) {

	//************Tutaj umieszczaj kod, który należy wykonać po zakończeniu pętli głównej************
	delete sp;
}


void initGameObjectTables() {
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			for (int k = 0; k < 15; k++)
			{
				gameTable[i][j][k] = 0;
				currentObject[i][j][k] = 0;
				currentObject_cp[i][j][k] = 0;
				shadow[i][j][k] = 0;
				tetromino1[i][j][k] = 0;
				tetromino2[i][j][k] = 0;
				tetromino3[i][j][k] = 0;
				tetromino4[i][j][k] = 0;
				tetromino5[i][j][k] = 0;
			}
		}
	}

	tetromino1[6][6][14] = 1;
	tetromino1[7][6][14] = 1;
	tetromino1[5][6][14] = 1;
	tetromino1[6][6][13] = 1;

	tetromino2[4][4][13] = 2;
	tetromino2[4][5][13] = 2;
	tetromino2[5][4][13] = 2;
	tetromino2[5][5][13] = 2;
	tetromino2[4][4][14] = 2;
	tetromino2[4][5][14] = 2;
	tetromino2[5][4][14] = 2;
	tetromino2[5][5][14] = 2;

	tetromino3[5][5][13] = 3;
	tetromino3[6][5][13] = 3;
	tetromino3[6][5][12] = 3;
	tetromino3[7][5][12] = 3;

	tetromino4[6][6][14] = 4;
	tetromino4[6][6][13] = 4;
	tetromino4[6][6][12] = 4;
	tetromino4[6][6][11] = 4;

	tetromino5[6][6][14] = 5;
	tetromino5[6][6][13] = 5;
	tetromino5[6][6][12] = 5;
	tetromino5[7][6][12] = 5;

}

//Procedura rysująca zawartość sceny

void drawFloor() {
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex_floor);

	glm::mat4 I = glm::mat4(1.0f);

	for (float i = -4.5; i <= 4.5; i++)
	{
		for (float j = -4.5; j <= 4.5; j++)
		{
			glm::mat4 M1 = glm::translate(I, glm::vec3(i, -0.5f, j));
			M1 = glm::scale(M1, glm::vec3(0.5f, 0.5f, 0.5f));
			glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M1));
			glUniform1i(sp->u("textureMap0"), 0);
			glDrawArrays(GL_TRIANGLES, 0, vertexCount);
		}
	}



	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			for (int k = 0; k < 15; k++) {
				if (tetrominoId == 2) {
					glBindTexture(GL_TEXTURE_2D, tex2);
					if (tetromino2[i][j][k]!=0) {
						glm::mat4 M1 = glm::translate(I, glm::vec3(i+7, k-15, j-11));
						//(11.0f, 1.0f, -5.0f));
						M1 = glm::scale(M1, glm::vec3(0.5f, 0.5f, 0.5f));
						glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M1));
						glUniform1i(sp->u("textureMap0"), 0);
						glDrawArrays(GL_TRIANGLES, 0, vertexCount);
					}	
				}
				if (tetrominoId == 3) {
					if (tetromino3[i][j][k] != 0) {
						glBindTexture(GL_TEXTURE_2D, tex3);
						glm::mat4 M1 = glm::translate(I, glm::vec3(i + 5, k - 14, j - 11));
						//(11.0f, 1.0f, -5.0f));
						M1 = glm::scale(M1, glm::vec3(0.5f, 0.5f, 0.5f));
						glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M1));
						glUniform1i(sp->u("textureMap0"), 0);
						glDrawArrays(GL_TRIANGLES, 0, vertexCount);
					}
				}
				if (tetrominoId == 4) {
					if (tetromino4[i][j][k] != 0) {
						glBindTexture(GL_TEXTURE_2D, tex4);
						glm::mat4 M1 = glm::translate(I, glm::vec3(i + 5, k - 14, j - 11));
						//(11.0f, 1.0f, -5.0f));
						M1 = glm::scale(M1, glm::vec3(0.5f, 0.5f, 0.5f));
						glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M1));
						glUniform1i(sp->u("textureMap0"), 0);
						glDrawArrays(GL_TRIANGLES, 0, vertexCount);
					}
				}
				if (tetrominoId == 5) {
					if (tetromino5[i][j][k] != 0) {
						glBindTexture(GL_TEXTURE_2D, tex5);
						glm::mat4 M1 = glm::translate(I, glm::vec3(i + 5, k - 14, j - 11));
						//(11.0f, 1.0f, -5.0f));
						M1 = glm::scale(M1, glm::vec3(0.5f, 0.5f, 0.5f));
						glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M1));
						glUniform1i(sp->u("textureMap0"), 0);
						glDrawArrays(GL_TRIANGLES, 0, vertexCount);
					}
				}
				if (tetrominoId == 1) {
					if (tetromino1[i][j][k] != 0) {
						glm::mat4 M1 = glm::translate(I, glm::vec3(i + 5, k - 14, j - 11));
						//(11.0f, 1.0f, -5.0f));
						M1 = glm::scale(M1, glm::vec3(0.5f, 0.5f, 0.5f));
						glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M1));
						glUniform1i(sp->u("textureMap0"), 0);
						glDrawArrays(GL_TRIANGLES, 0, vertexCount);
					}
				}
			}
		}
	}

}


void drawScene(GLFWwindow* window) {
	//************Tutaj umieszczaj kod rysujący obraz******************l
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Wyczyszczenie bufora kolorów i bufora głębokości

	glm::mat4 P = glm::perspective(50.0f * PI / 180.0f, aspectRatio, 0.01f, 50.0f); //Wyliczenie macierzy rzutowania
	glm::mat4 V = glm::lookAt(glm::vec3(3.0f, 20.0f, 15.0f), glm::vec3(3.0f, 5.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)); //Wyliczenie macierzy widoku

	glUniform1i(sp->u("textureMap0"), 0);

	sp->use();//Aktywacja programu cieniującego

	glUniformMatrix4fv(sp->u("P"), 1, false, glm::value_ptr(P)); //Załadowanie macierzy rzutowania do programu cieniującego
	glUniformMatrix4fv(sp->u("V"), 1, false, glm::value_ptr(V)); //Załadowanie macierzy widoku do programu cieniującego

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex_letters);
	glm::mat4 M = glm::mat4(1.0f);
	M = glm::translate(M, glm::vec3(5.5f, 11.0f, 4.0f));
	M = glm::scale(M, glm::vec3(0.02f, 0.02f, 0.02f));
	glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M));
	glEnableVertexAttribArray(sp->a("vertex"));
	glVertexAttribPointer(sp->a("vertex"), 4, GL_FLOAT, false, 0, verts_N.data());
	glEnableVertexAttribArray(sp->a("normal"));
	glVertexAttribPointer(sp->a("normal"), 4, GL_FLOAT, false, 0, norms_N.data());
	glEnableVertexAttribArray(sp->a("texCoord0"));
	glVertexAttribPointer(sp->a("texCoord0"), 2, GL_FLOAT, false, 0, texCoords1_N.data());
	glDrawElements(GL_TRIANGLES, Indices_N.size(), GL_UNSIGNED_INT, Indices_N.data());
	
	M = glm::mat4(1.0f);
	M = glm::translate(M, glm::vec3(6.5f, 11.0f, 4.0f));
	M = glm::scale(M, glm::vec3(0.02f, 0.02f, 0.02f));
	glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M));
	glEnableVertexAttribArray(sp->a("vertex"));
	glVertexAttribPointer(sp->a("vertex"), 4, GL_FLOAT, false, 0, verts_E.data());
	glEnableVertexAttribArray(sp->a("normal"));
	glVertexAttribPointer(sp->a("normal"), 4, GL_FLOAT, false, 0, norms_E.data());
	glEnableVertexAttribArray(sp->a("texCoord0"));
	glVertexAttribPointer(sp->a("texCoord0"), 2, GL_FLOAT, false, 0, texCoords1_E.data());
	glDrawElements(GL_TRIANGLES, Indices_E.size(), GL_UNSIGNED_INT, Indices_E.data());

	M = glm::mat4(1.0f);
	M = glm::translate(M, glm::vec3(7.7f, 11.0f, 4.0f));
	M = glm::scale(M, glm::vec3(0.02f, 0.02f, 0.02f));
	glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M));
	glEnableVertexAttribArray(sp->a("vertex"));
	glVertexAttribPointer(sp->a("vertex"), 4, GL_FLOAT, false, 0, verts_X.data());
	glEnableVertexAttribArray(sp->a("normal"));
	glVertexAttribPointer(sp->a("normal"), 4, GL_FLOAT, false, 0, norms_X.data());
	glEnableVertexAttribArray(sp->a("texCoord0"));
	glVertexAttribPointer(sp->a("texCoord0"), 2, GL_FLOAT, false, 0, texCoords1_X.data());
	glDrawElements(GL_TRIANGLES, Indices_X.size(), GL_UNSIGNED_INT, Indices_X.data());

	M = glm::mat4(1.0f);
	M = glm::translate(M, glm::vec3(8.9f, 11.6f, 4.0f));
	M = glm::scale(M, glm::vec3(0.02f, 0.02f, 0.02f));
	glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M));
	glEnableVertexAttribArray(sp->a("vertex"));
	glVertexAttribPointer(sp->a("vertex"), 4, GL_FLOAT, false, 0, verts_T.data());
	glEnableVertexAttribArray(sp->a("normal"));
	glVertexAttribPointer(sp->a("normal"), 4, GL_FLOAT, false, 0, norms_T.data());
	glEnableVertexAttribArray(sp->a("texCoord0"));
	glVertexAttribPointer(sp->a("texCoord0"), 2, GL_FLOAT, false, 0, texCoords1_T.data());
	glDrawElements(GL_TRIANGLES, Indices_T.size(), GL_UNSIGNED_INT, Indices_T.data());

	glEnableVertexAttribArray(sp->a("vertex"));  //Włącz przesyłanie danych do atrybutu vertex
	glVertexAttribPointer(sp->a("vertex"), 4, GL_FLOAT, false, 0, vertices);

	glEnableVertexAttribArray(sp->a("color"));  //Włącz przesyłanie danych do atrybutu color
	glVertexAttribPointer(sp->a("color"), 4, GL_FLOAT, false, 0, colors); //Wskaż tablicę z danymi dla atrybutu color

	glEnableVertexAttribArray(sp->a("normal"));  //Włącz przesyłanie danych do atrybutu normal
	glVertexAttribPointer(sp->a("normal"), 4, GL_FLOAT, false, 0, normals); //Wskaż tablicę z danymi dla atrybutu normal

	glEnableVertexAttribArray(sp->a("texCoord0"));  //Włącz przesyłanie danych do atrybutu texCoord
	glVertexAttribPointer(sp->a("texCoord0"), 2, GL_FLOAT, false, 0, texCoords);


	drawFloor(); 

	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			for (float k = 0; k < 15; k++)
			{
				int k1 = k;
				if (gameTable[i][j][k1] != 0) {
					glm::mat4 M1 = glm::mat4(1.0f);
					objX = i - 4.5;
					objZ = j - 4.5;
					M1 = glm::translate(M1, glm::vec3(objX, k + 0.5, -objZ));
					M1 = glm::scale(M1, glm::vec3(0.5f, 0.5f, 0.5f));
					glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M1));

					if (gameTable[i][j][k1] == 1) glBindTexture(GL_TEXTURE_2D, tex1);
					if (gameTable[i][j][k1] == 2) glBindTexture(GL_TEXTURE_2D, tex2);
					if (gameTable[i][j][k1] == 3) glBindTexture(GL_TEXTURE_2D, tex3);
					if (gameTable[i][j][k1] == 4) glBindTexture(GL_TEXTURE_2D, tex4);
					if (gameTable[i][j][k1] == 5) glBindTexture(GL_TEXTURE_2D, tex5);


					glUniform1i(sp->u("textureMap0"), 0);
					glDrawArrays(GL_TRIANGLES, 0, vertexCount);
				}
				if (shadow[i][j][k1] != 0) {
					glBindTexture(GL_TEXTURE_2D, tex_shadow);
					glEnable(GL_BLEND);
					glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
					glm::mat4 M1 = glm::mat4(1.0f);
					objX = i - 4.5;
					objZ = j - 4.5;
					M1 = glm::translate(M1, glm::vec3(objX, k + 0.5, -objZ));
					M1 = glm::scale(M1, glm::vec3(0.5f, 0.5f, 0.5f));
					glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M1));
					glUniform1i(sp->u("textureMap0"), 0);
					glDrawArrays(GL_TRIANGLES, 0, vertexCount);

				}
			}
		}
	}

	glDisableVertexAttribArray(sp->a("vertex"));  //Wyłącz przesyłanie danych do atrybutu vertex  //Wyłącz przesyłanie danych do atrybutu normal
	glDisableVertexAttribArray(sp->a("color"));  //Wyłącz przesyłanie danych do atrybutu color
	glDisableVertexAttribArray(sp->a("normal"));  //Wyłącz przesyłanie danych do atrybutu normal
	glDisableVertexAttribArray(sp->a("texCoord0"));

	//Skopiowanie bufora ukrytego do widocznego. Z reguły ostatnie polecenie w procedurze drawScene.
	glfwSwapBuffers(window);
}

void calculateShadow() {
	int height = 15;
	int minHeight = 15;
	bool gotMinObj = false;
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			for (int k = 0; k < 15; k++)
			{
				if (gameTable[i][j][k] != 0 && currentObject[i][j][k] == 0) height = 14 - k;
				if (currentObject[i][j][k] != 0 && gotMinObj == false) {

					height = height - (15 - k);
					gotMinObj = true;
					if (height < minHeight) {
						minHeight = height;
					}
				}
			}
			height = 15;
			gotMinObj = false;
		}
	}

	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			for (int k = 0; k < 15; k++)
			{
				{
					shadow[i][j][k - minHeight] = currentObject[i][j][k];
				}
			}
		}
	}
}

int main(void)
{
	GLFWwindow* window; //Wskaźnik na obiekt reprezentujący okno

	glfwSetErrorCallback(error_callback);//Zarejestruj procedurę obsługi błędów

	if (!glfwInit()) { //Zainicjuj bibliotekę GLFW
		fprintf(stderr, "Nie można zainicjować GLFW.\n");
		exit(EXIT_FAILURE);
	}

	window = glfwCreateWindow(800, 800, "OpenGL", NULL, NULL);  //Utwórz okno 800x800 o tytule "OpenGL" i kontekst OpenGL.

	if (!window) //Jeżeli okna nie udało się utworzyć, to zamknij program
	{
		fprintf(stderr, "Nie można utworzyć okna.\n");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window); //Od tego momentu kontekst okna staje się aktywny i polecenia OpenGL będą dotyczyć właśnie jego.
	glfwSwapInterval(1); //Czekaj na 1 powrót plamki przed pokazaniem ukrytego bufora

	if (glewInit() != GLEW_OK) { //Zainicjuj bibliotekę GLEW
		fprintf(stderr, "Nie można zainicjować GLEW.\n");
		exit(EXIT_FAILURE);
	}

	initOpenGLProgram(window); //Operacje inicjujące

	initGameObjectTables();

	objY = 14.5f;
	float objY_round = floor(objY);
	float obj_round_2 = 0;

	glfwSetTime(0); //Wyzeruj timer
	//Główna pętla
	while (!glfwWindowShouldClose(window))
	{
		calculateShadow();
		if (!con)
		{
			for (int i = 0; i < 10; i++)
			{
				for (int j = 0; j < 10; j++)
				{
					for (int k = 0; k < 15; k++)
					{
						if (tetrominoId == 1) currentObject[i][j][k] = tetromino1[i][j][k];
						if (tetrominoId == 2) currentObject[i][j][k] = tetromino2[i][j][k];
						if (tetrominoId == 3) currentObject[i][j][k] = tetromino3[i][j][k];
						if (tetrominoId == 4) currentObject[i][j][k] = tetromino4[i][j][k];
						if (tetrominoId == 5) currentObject[i][j][k] = tetromino5[i][j][k];
						
					}
				}
			}
			tetrominoId++;
			if (tetrominoId == 6) tetrominoId = 1;
			con = true;
		}


		objY += -1.0 * glfwGetTime();
		objY_round = floor(objY);

		for (int i = 0; i < 10; i++)
		{
			for (int j = 0; j < 10; j++)
			{
				for (int k = 0; k < 15; k++)
				{
					currentObject_cp[i][j][k] = currentObject[i][j][k];
				}
			}
		}
		
		if (objY_round != obj_round_2)
		{
			
			bool canMove = true;


			for (int i = 0; i < 10; i++)
			{
				for (int j = 0; j < 10; j++)
				{
					if (currentObject[i][j][0] != 0)
					{
						canMove = false;
						con = false;
					}
						
				}
			}

			for (int i = 0; i < 10; i++)
			{
				for (int j = 0; j < 10; j++)
				{
					for (int k = 1; k < 15; k++)
					{
						if (currentObject[i][j][k] != 0 && gameTable[i][j][k] != 0 &&
							currentObject[i][j][k - 1] == 0 && gameTable[i][j][k - 1] != 0)
						{
							canMove = false;
							con = false;
						}

					}
				}
			}

			if (canMove)
			{
				for (int i = 0; i < 10; i++)
				{
					for (int j = 0; j < 10; j++)
					{
						for (int k = 0; k < 15; k++)
						{
							currentObject_cp[i][j][k] = currentObject[i][j][k];
						}
					}
				}
				for (int i = 0; i < 10; i++)
				{
					for (int j = 0; j < 10; j++)
					{
						for (int k = 0; k < 15; k++)
						{
							currentObject[i][j][k] = currentObject_cp[i][j][k+1];
							if (currentObject[i][j][k] != 0)
							{
								gameTable[i][j][k] = currentObject[i][j][k];
								gameTable[i][j][k + 1] = 0;
							}
						}
					}
				}
			}
		}
		obj_round_2 = objY_round;

		glfwSetTime(0); //Wyzeruj timer
		drawScene(window); //Wykonaj procedurę rysującą
		glfwPollEvents(); //Wykonaj procedury callback w zalezności od zdarzeń jakie zaszły.
	}

	freeOpenGLProgram(window);

	glfwDestroyWindow(window); //Usuń kontekst OpenGL i okno
	glfwTerminate(); //Zwolnij zasoby zajęte przez GLFW
	exit(EXIT_SUCCESS);
}
