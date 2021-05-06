#pragma once
#include <QtWidgets>


class vektor {
	double x=0, y=0, z=0;
public:
	vektor() {};
	vektor(double X, double Y, double Z) { x = X; y = Y; z = Z; };

	void setAll(double X, double Y, double Z) { x = X; y = Y; z = Z; };
	void setX(double X) { x = X; };
	void setY(double Y) { y = Y; };
	void setZ(double Z) { z = Z; };

	double getX() { return x; };
	double getY() { return y; };
	double getZ() { return z; };
	double dotProduct(vektor v) { return x * v.getX() + y * v.getY() + z * v.getZ(); };
	double norma() { return sqrt(x * x + y * y + z * z); };
};

class H_Edge;

class Vertex {
	int index;
	double x, y, z;
	H_Edge* edge;
	vektor normala;
	QColor farba;
public:
	Vertex() {};
	Vertex(int i, double surX, double surY, double surZ) { index = i; x = surX; y = surY; z = surZ; };

	void set(int i, double surX, double surY, double surZ) { index = i; x = surX; y = surY; z = surZ; };
	void setEdge(H_Edge* E) { edge = E; };
	void setSur(double surX, double surY, double surZ) {  x = surX; y = surY; z = surZ; };
	void setIndex(int i) { index = i; };
	void setX(double surX) { x = surX; };
	void setY(double surY) { y = surY; };
	void setZ(double surZ) { z = surZ; };
	void setNormala() {
		double dlzka = sqrt(x * x + y * y + z * z);
		normala.setAll(x / dlzka, y / dlzka, z / dlzka);
	};
	void setFarba(int r, int g, int b) { farba.setRed(r); farba.setGreen(g); farba.setBlue(b); };
	void setFarba(QColor f) { farba = f; };

	QString getSur() { return QString::number(x) + " " + QString::number(y) + " " + QString::number(z) + " "; };
	int getIndex() { return index; };
	double getX() { return x; };
	double getY() { return y; };
	double getZ() { return z; };
	vektor getNormala() { return normala; };
	QColor getFarba() { return farba; };

};

class Face;

class H_Edge {
	Vertex* vert_origin;
	Vertex* vert_end;
	Face* face;
	H_Edge* edge_prev, * edge_next;
	H_Edge* pair;
public:
	H_Edge() { face = nullptr; };
	H_Edge(Vertex* vo, Face* f, H_Edge* ep, H_Edge* en, H_Edge* pr) { vert_origin = vo; face = f; edge_prev = ep; edge_next = en; pair = pr; };

	
	void set(Vertex* vo, Face* f, H_Edge* ep, H_Edge* en, H_Edge* pr = nullptr) { vert_origin = vo; face = f; edge_prev = ep; edge_next = en; pair = pr; };
	void setVert(Vertex* vo, Vertex* ve) { vert_origin = vo; vert_end = ve; };
	void setFace(Face* f) { face = f; };
	void setEdgePrev(H_Edge* ep) { edge_prev = ep; };
	void setEdgeNext(H_Edge* en) { edge_next = en; };
	void setPair(H_Edge* pr) { pair = pr; };

	QString PrintVrcholyHrany() { return QString::number(vert_origin->getIndex()) + " " + QString::number(edge_next->vert_origin->getIndex()) + " "; };
	QString PrintVrcholySteny() {return QString::number(vert_origin->getIndex()) + " " +QString::number(edge_next->vert_origin->getIndex()) + " " + QString::number(edge_prev->vert_origin->getIndex());
	};
	Face* getStena() { return face; };

	int getVOIndex() { return vert_origin->getIndex(); };
	int getVEIndex() { return vert_end->getIndex(); };
	Vertex* getVertexO() { return vert_origin; };
	H_Edge* getHrana_next() { return edge_next; };
	H_Edge* getHrana_prev() { return edge_prev; };
	H_Edge* getHrana_pair() { return pair; };
};

class Face {
	H_Edge* edge=nullptr;
public:
	Face() {};
	Face(H_Edge* E) { edge = E; };

	void setEdge(H_Edge* E) { edge = E; };
	H_Edge* getEdge() { return edge; };

	QString PrintVrcholy() { return edge->PrintVrcholySteny(); };
};



class Hedron {
	QList<Vertex>* Vrcholy=NULL;
	QList<H_Edge>* Hrany=NULL;
	QList<Face>* Steny=NULL;
public:
	void setVrcholy(QList<Vertex>* vrcholy) { Vrcholy = vrcholy; };
	void setHrany(QList<H_Edge>* hrany) { Hrany = hrany; };
	void setSteny(QList<Face>* steny) { Steny = steny; };
	QList<Vertex>* getVrcholy() { return Vrcholy; };
	QList<H_Edge>* getHrany() { return Hrany; };
	QList<Face>* getSteny() { return Steny; };
	Face* getStena(int i) { return &(*Steny)[i]; };
	void setParove() {
		int i, j;
		for (i = 0; i < getHranysize(); i++) {
			for (j = 0; j < getHranysize(); j++) {
				if ((*Hrany)[i].getVOIndex() == (*Hrany)[j].getHrana_next()->getVOIndex() && (*Hrany)[i].getHrana_next()->getVOIndex() == (*Hrany)[j].getVOIndex()){
					(*Hrany)[i].setPair(&(*Hrany)[j]);
					(*Hrany)[j].setPair(&(*Hrany)[i]);
				}
			}
		}
	};
	int getVrcholysize() { return Vrcholy->size(); };
	int getHranysize() { return Hrany->size(); };
	int getStenysize() { return Steny->size(); };
	int getHranaOIndex(int i) { return (*Hrany)[i].getVOIndex(); };
	int getPairHranaOIndex(int i) { return (*Hrany)[i].getHrana_pair()->getVOIndex(); };
	QString printSur(int i) { return (*Vrcholy)[i].getSur(); };
	QString printHrana(int i) { return (*Hrany)[i].PrintVrcholyHrany(); };
	QString printStena(int i) { return (*Steny)[i].PrintVrcholy(); };
	bool HisEmpty() { 
		if (Vrcholy == NULL && Hrany == NULL && Steny == NULL) return true;
		else return false;
	};
	void clear() { Vrcholy->clear(); Hrany->clear(); Steny->clear(); Vrcholy = NULL; Hrany = NULL; Steny = NULL; };
	void setNormalaVetrexu(int i) { (*Vrcholy)[i].setNormala(); };
};


class premietanie {
	double zenit = 0, azimut = 0;
	int typ = 0, Far = 0, Near = 0;

	double nx, ny, nz;
	double ux, uy, uz;
	double vx, vy, vz;


public:
	premietanie() {};
	void setZenit(float zen) { zenit = zen; };
	void setAzimut(float azi) { azimut = azi; };
	void setTyp(int a) { typ = a; };
	void setFar(int a) { Far = a; };
	void setNear(int a) { Near = a; };
	void setN() {
		nx = qSin(zenit) * qCos(azimut);
		ny = qSin(zenit) * qSin(azimut);
		nz = qCos(zenit);
	};
	void setU() {
		ux = qSin(zenit + M_PI / 2) * qCos(azimut);
		uy = qSin(zenit + M_PI / 2) * qSin(azimut);
		uz = qCos(zenit + M_PI / 2);
	};
	void setV() {
		vx = ny * uz - nz * uy;
		vy = nz * ux - nx * uz;
		vz = nx * uy - ny * ux;
	}

	float getAzimut() { return azimut; };
	float getZenit() { return zenit; };
	int getTyp() { return typ; };
	int getFar() { return Far; };
	int getNear() { return Near; };
	double getnx() { return nx; };
	double getny() { return ny; };
	double getnz() { return nz; };
	double getux() { return ux; };
	double getuy() { return uy; };
	double getuz() { return uz; };
	double getvx() { return vx; };
	double getvy() { return vy; };
	double getvz() { return vz; };
	vektor getN() { vektor vekt(nx, ny, nz); return vekt; };
};