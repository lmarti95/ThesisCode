#include "MST_Visualization.h"

#include <fstream>
#include <iostream>
#include <random>
#include <utility>

MST_Visualization::MST_Visualization(MST* aCostFunction, EvolutionaryAlgorithm* aEA)
{
	mEA = aEA;
	mCostFunction = aCostFunction;
	mCircles = new std::vector<Circle*>;

	if(!ReadInCircles())
	{
		GenerateCircles();
		SaveCircles();
	}

	mNumberOfTriangles = mCircles->size() * mCircles->at(0)->GetNumberOfTriangles();
	auto lines = CreateLines();
	mNumberOfLines = lines->size();

	for(auto& l : *lines)
	{
		delete l;
	}
	delete lines;
}

MST_Visualization::~MST_Visualization()
{
	for(auto& c : *mCircles)
	{
		delete c;
	}

	delete mCircles;
}

std::pair<GLfloat, GLfloat> MST_Visualization::GenerateRandomCoordinates()
{
	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_real_distribution<> p(-0.9, 0.6);

	GLfloat x = p(rng);
	GLfloat y = p(rng);

	return std::make_pair(x,y);
}

bool MST_Visualization::CheckCoordinates(std::pair<GLfloat, GLfloat> aCoordinates)
{
	for(auto& c : *mCircles)
	{
		auto points = dynamic_cast<Circle*>(c)->GetCenter();
		if(points.first-0.1 < aCoordinates.first && points.first + 0.1 > aCoordinates.first)
		{
			if(points.second - 0.1 < aCoordinates.second && points.second + 0.1 > aCoordinates.second)
			{
				return false;
			}
		}
	}

	return true;
}

void MST_Visualization::GenerateCircles()
{
	mCircles->clear();
	GLfloat red[3] = {1.0f, 0.0f, 0.0f};

	for(int i = 0; i < mCostFunction->GetNodesNum(); ++i)
	{
		auto cords = GenerateRandomCoordinates();

		while(!CheckCoordinates(cords))
		{
			cords = GenerateRandomCoordinates();
		}

		Circle* c = new Circle(cords.first, cords.second, 0.03f, red);
		c->SetID(i);
		mCircles->push_back(c);
	}	
}

void MST_Visualization::SaveCircles()
{
	std::ofstream file;
	int nodes = mCostFunction->GetNodesNum();
	file.open(std::to_string(nodes) + ".vis");
	for(auto& c : *mCircles)
	{
		file << c->GetCenter().first << " " << c->GetCenter().second << std::endl;
	}

	file.close();
}

bool MST_Visualization::ReadInCircles()
{
	std::ifstream file;
	int nodes = mCostFunction->GetNodesNum();
	file.open(std::to_string(nodes) + ".vis");

	if(!file.is_open())
	{
		return false;
	}

	GLfloat red[3] = {1.0f, 0.0f, 0.0f};

	for(int i = 0; i < mCostFunction->GetNodesNum(); ++i)
	{
		GLfloat x, y;
		file >> x >> y;

		Circle* c = new Circle(x, y, 0.03f, red);
		c->SetID(i);
		mCircles->push_back(c);
	}

	file.close();

	return true;
}

GLfloat* MST_Visualization::GetVertices()
{
	GLfloat* vertices = new GLfloat[mNumberOfTriangles*9 + mNumberOfLines*6];

	int copied = 0;
	for(auto& c : *mCircles)
	{
		auto points = c->GetPoints();
		for(int i = 0; i < c->GetNumberOfTriangles() * 9; ++i)
		{
			vertices[copied + i] = points->at(i);
		}
		copied += c->GetNumberOfTriangles() * 9;

		delete points;
	}

	auto lines = CreateLines();
	for(auto& l : *lines)
	{
		auto points = l->GetPoints();
		for(int i = 0; i < 6; ++i)
		{
			vertices[copied + i] = points->at(i);
		}
		copied += 6;

		delete points;
	}

	for(auto& l : *lines)
	{
		delete l;
	}
	delete lines;

	return vertices;
}

GLfloat* MST_Visualization::GetColor()
{
	GLfloat* color = new GLfloat[mNumberOfTriangles * 9 + mNumberOfLines * 6];

	int copied = 0;

	for(auto& c : *mCircles)
	{
		auto colorPoints = c->GetColor();
		for(int i = 0; i < c->GetNumberOfTriangles() * 9; ++i)
		{
			color[copied + i] = colorPoints->at(i);
		}
		copied += c->GetNumberOfTriangles() * 9;

		delete colorPoints;
	}

	auto lines = CreateLines();
	for(auto& l : *lines)
	{
		auto colorPoints = l->GetColor();
		for(int i = 0; i < 6; ++i)
		{
			color[copied + i] = colorPoints->at(i);
		}
		copied += 6;

		delete colorPoints;
	}

	for(auto& l : *lines)
	{
		delete l;
	}
	delete lines;

	return color;
}

std::vector<Shape*>* MST_Visualization::CreateLines()
{
	std::vector<Shape*>* lines = new std::vector<Shape*>;
	GLfloat black[6] = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
	GLfloat red[6] = {1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f};

	auto bitString = mEA->GetBitString();

	for(int i = 0; i < mCostFunction->GetEdges()->size();++i)
	{
		auto e = mCostFunction->GetEdges()->at(i);
		auto c1 = GetCircle(e.n1);
		auto c2 = GetCircle(e.n2);

		Line* line;

		if(bitString->at(i) == 0)
		{
			line = new Line(c1->GetCenter().first, c1->GetCenter().second, c2->GetCenter().first, c2->GetCenter().second, black);
		}
		else
		{
			line = new Line(c1->GetCenter().first, c1->GetCenter().second, c2->GetCenter().first, c2->GetCenter().second, red);
		}

		lines->push_back((Shape*)line);
	}

	return lines;
}

Circle* MST_Visualization::GetCircle(int aID)
{
	for(auto& c : *mCircles)
	{
		if(c->GetiD() == aID)
		{
			return c;
		}
	}

	return nullptr;
}