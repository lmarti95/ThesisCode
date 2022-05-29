#include "MSTVisualization.h"

#if GRAPHICS

#include "OpenGL.h"

#include <fstream>
#include <iostream>
#include <random>
#include <utility>

MSTVisualization::MSTVisualization(EvolutionaryAlgorithm* aEA)
{
	mEA = aEA;
	mCostFunction = dynamic_cast<MST*>(aEA->GetCostFunction());
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

MSTVisualization::~MSTVisualization()
{
	for(auto& c : *mCircles)
	{
		delete c;
	}

	delete mCircles;
}

std::pair<GLfloat, GLfloat> MSTVisualization::GenerateRandomCoordinates()
{
	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_real_distribution<> p(-0.95, 0.9);

	GLfloat x = p(rng);
	GLfloat y = p(rng);

	while(x > 0.6 && y > 0.6)
	{
		x = p(rng);
		y = p(rng);
	}

	return std::make_pair(x,y);
}

bool MSTVisualization::CheckCoordinates(std::pair<GLfloat, GLfloat> aCoordinates)
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

void MSTVisualization::GenerateCircles()
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

void MSTVisualization::SaveCircles()
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

bool MSTVisualization::ReadInCircles()
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

GLfloat* MSTVisualization::GetVertices()
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

GLfloat* MSTVisualization::GetColor()
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

std::vector<Shape*>* MSTVisualization::CreateLines()
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
			line = new Line(c1->GetCenter().first+2, c1->GetCenter().second+2, c2->GetCenter().first+2, c2->GetCenter().second+2, black);
		}
		else
		{
			line = new Line(c1->GetCenter().first, c1->GetCenter().second, c2->GetCenter().first, c2->GetCenter().second, red);
		}

		lines->push_back((Shape*)line);

	}

	
	delete bitString;

	return lines;
}

Circle* MSTVisualization::GetCircle(int aID)
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

std::vector<Text> MSTVisualization::CreateWeighTexts()
{
	std::vector<Text> WeightText;

	for(int i = 0; i < mCostFunction->GetEdges()->size(); ++i)
	{
		auto e = mCostFunction->GetEdges()->at(i);
		GLTtext* t = gltCreateText();
		gltSetText(t, std::to_string(e.weight).c_str());

		auto c1 = GetCircle(e.n1);
		auto c2 = GetCircle(e.n2);

		double dx;
		double dy;

		if(c1->GetCenter().first < c2->GetCenter().first)
		{
			dx = c2->GetCenter().first - c1->GetCenter().first;
			dy = c2->GetCenter().second - c1->GetCenter().second;
		}
		else
		{
			dx = c1->GetCenter().first - c2->GetCenter().first;
			dy = c1->GetCenter().second - c2->GetCenter().second;
		}

		if(dx > dy && dx > dy * -1)
		{
			if(dx > 0)
			{
				dy = dy / dx*0.05;
				dx = 0.05;
			}
			else
			{
				dy = dy / dx * -0.05;
				dx = -0.05;
			}
		}
		else
		{
			if(dy > 0)
			{
				dx = dx / dy * 0.05;
				dy = 0.05;
			}
			else
			{
				dx = dx / dy * -0.05;
				dy = -0.05;
			}
		}

		double dx2 = dx;
		double dy2 = dy;

		if(c1->GetCenter().first < c2->GetCenter().first)
		{
			dx = (c2->GetCenter().first - c1->GetCenter().first)/2 + c1->GetCenter().first -dy2;
		}
		else
		{
			dx = (c1->GetCenter().first - c2->GetCenter().first) / 2 + c2->GetCenter().first - dy2;
		}

		if(c1->GetCenter().second < c2->GetCenter().second)
		{
			dy = (c2->GetCenter().second - c1->GetCenter().second) / 2 + c1->GetCenter().second + dx2;
		}
		else
		{
			dy = (c1->GetCenter().second - c2->GetCenter().second) / 2 + c2->GetCenter().second + dx2;
		}

		WeightText.push_back(Text{t, TranslateCoordinateX(dx), TranslateCoordinateY(dy)});
	}

	return WeightText;
}

float MSTVisualization::TranslateCoordinateX(double aX)
{
	return (aX + 1) / 2 * OpenGL::GetWidth();
}

float MSTVisualization::TranslateCoordinateY(double aY)
{
	return (-aY + 1) / 2 * OpenGL::GetHeight();
}

#endif