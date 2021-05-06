#include   "ViewerWidget.h"

ViewerWidget::ViewerWidget(QString viewerName, QSize imgSize, QWidget* parent)
	: QWidget(parent)
{
	setAttribute(Qt::WA_StaticContents);
	setMouseTracking(true);
	name = viewerName;
	if (imgSize != QSize(0, 0)) {
		img = new QImage(imgSize, QImage::Format_ARGB32);
		img->fill(Qt::white);
		resizeWidget(img->size());
		setPainter();
		setDataPtr();
	}
}
ViewerWidget::~ViewerWidget()
{
	delete painter;
	delete img;
}
void ViewerWidget::resizeWidget(QSize size)
{
	this->resize(size);
	this->setMinimumSize(size);
	this->setMaximumSize(size);
}

//Image functions
bool ViewerWidget::setImage(const QImage& inputImg)
{
	if (img != nullptr) {
		delete img;
	}
	img = new QImage(inputImg);
	if (!img) {
		return false;
	}
	resizeWidget(img->size());
	setPainter();
	update();

	return true;
}
bool ViewerWidget::isEmpty()
{
	if (img->size() == QSize(0, 0)) {
		return true;
	}
	return false;
}

//Data function
void ViewerWidget::setPixel(int x, int y, const QColor& color)
{
	if (isInside(x, y))
		data[x + y * img->width()] = color.rgb();
	else
		qDebug() << "nechce vykreslit bod" << x << "," << y << "\n";
}
void ViewerWidget::setPixel(int x, int y, unsigned char r, unsigned char g, unsigned char b)
{
	if (isInside(x, y)) {
		QColor color(r, g, b);
		setPixel(x, y, color);
	}
}

void ViewerWidget::clear(QColor color)
{
	for (size_t x = 0; x < img->width(); x++)
	{
		for (size_t y = 0; y < img->height(); y++)
		{
			setPixel(x, y, color);
		}
	}
	update();
}



//Draw functions
void ViewerWidget::freeDraw(QPoint end, QPen pen)
{
	painter->setPen(pen);
	painter->drawLine(freeDrawBegin, end);

	update();
}

void ViewerWidget::clear()
{
	img->fill(Qt::white);
	update();
}



void ViewerWidget::usecka_DDA(QPointF A, QPointF B, QColor color) {
	int x, y, x2, y2;
	double chyba = 0;
	if (B.y() == A.y()) {
		int delta = abs(A.x() - B.x()), x1;
		if (A.x() < B.x())
			x1 = A.x();
		else
			x1 = B.x();
		for (x = 0; x < delta; x++)
			setPixel((int)x + (int)x1, (int)A.y(), color);
	}
	else {
		double smernica = (B.y() - A.y()) / (double)(B.x() - A.x());
		double posun = 0;
		if (abs(smernica) < 1) {											//riadiaca os x
			if (A.x() < B.x()) {
				x = A.x(); y = A.y(); x2 = B.x();
			}
			else if (A.x() > B.x()) {
				x = B.x(); y = B.y(); x2 = A.x();
			}
			else if (A.x() == B.x()) {
				int delta = abs(A.y() - B.y()), y1;
				if (A.y() < B.y())
					y1 = A.y();
				else
					y1 = B.y();
				for (y = 0; y < delta; y++)
					setPixel((int) (y + y1), (int)A.x(), color);
			}

			for (x; x < x2; x++) {
				setPixel((int)x, (int)y, color);
				posun += smernica;
				if (posun > 0.5) {
					y += (int)smernica;
					if ((int)smernica == 0)
						y++;
					posun += -1;
				}
				if (posun < -0.5) {
					y += -(int)smernica;
					if ((int)smernica == 0)
						y--;
					posun += 1;
				}
			}
		}
		else {																//riadiaca os y
			if (A.y() > B.y())
				y = A.y(), x = A.x(), y2 = B.y();
			else if (A.y() < B.y())
				y = B.y(), x = B.x(), y2 = A.y();
			for (y; y > y2; y--) {
				setPixel((int)x, (int)y, color);
				posun += 1 / smernica;
				if (posun > 0.5) {
					x--;
					posun += -1;
				}
				if (posun < -0.5) {
					x++;
					posun += 1;
				}
			}
		}
	}
	update();
}

void ViewerWidget::scanLineTri(QList <QPointF> body, QColor vypln, int algovypln, QList<QColor> farby) {
	//QVector<edge> hrany (3);
																//usporiadanie pola bodov
	int i, j, k, y = 0, Ymax = 0;
	float Xmin = 0, Xmax = 0, m;
	qDebug() << body;
	for (i = 0; i < 3; i++) {
		for (j = 0; j < 2; j++) {
			if (body[j].y() > body[j + 1].y()) {
				QPointF temp = body[j];
				body[j] = body[j + 1];
				body[j + 1] = temp;
			}
			else if (body[j].y() == body[j + 1].y()) {
				if (body[j].x() > body[j + 1].x()) {
					QPointF temp = body[j];
					body[j] = body[j + 1];
					body[j + 1] = temp;
				}
			}
		}
	}
	qDebug() << body;
	QVector<QPointF> e1(2), e2(2);								//vytvorenie "podtrojuholnikov"
	float w1, w2;
	bool ibajeden = false;
	if (body[0].y() == body[1].y()) {				//iba spodny
		ibajeden = true;
		e1[0] = body[0];
		e1[1] = body[2];

		e2[0] = body[1];
		e2[1] = body[2];

		Xmin = e1[0].x();
		Xmax = e2[0].x();

		y = e1[0].y();
		Ymax = e1[1].y();
	}
	else if (body[1].y() == body[2].y()) {			//iba vrchny
		ibajeden = true;
		e1[0] = body[0];
		e1[1] = body[1];

		e2[0] = body[0];
		e2[1] = body[2];

		Xmin = e1[0].x();
		Xmax = e2[0].x();

		y = e1[0].y();
		Ymax = e1[1].y();
	}
	else {
		float m = (body[2].y() - body[0].y()) / (float)(body[2].x() - body[0].x());
		int px = ((body[1].y() - body[0].y()) / m) + body[0].x();
		QPointF P;
		P.setX((int)px);
		P.setY(body[1].y());

		if (body[1].x() < P.x()) {
			QVector<QPointF> e3(2), e4(2);
			float w3, w4;
			e1[0] = body[0];														//nehorizontalne hrany 
			e1[1] = body[1];

			e2[0] = body[0];
			e2[1] = P;

			e3[0] = body[1];
			e3[1] = body[2];

			e4[0] = P;
			e4[1] = body[2];

			m = (e3[1].y() - e3[0].y()) / (double)(e3[1].x() - e3[0].x());			//nastavenie premennych pre dolny 
			w3 = 1.0 / m;
			m = (e4[1].y() - e4[0].y()) / (double)(e4[1].x() - e4[0].x());
			w4 = 1.0 / m;
			Xmin = e3[0].x();
			Xmax = e4[0].x();
			y = e3[0].y();
			Ymax = e3[1].y();

			int dx = Xmax - Xmin;
			while (y < Ymax) {														//kreslenie dolneho
				for (i = 1; i <= dx; i++) {
					if (algovypln == 0) {				//nearest neighbor
						setPixel((int)Xmin + i, (int)y, nearest(int(Xmin) + i, (int)y, body, farby));
					}
					else if (algovypln == 1) {			//barycentric
						setPixel((int)Xmin + i, (int)y, bary(int(Xmin) + i, (int)y, body, farby));
					}
				}
				Xmin += w3;
				Xmax += w4;
				y++;
				dx = Xmax - Xmin;
			}

			Xmin = e1[0].x();														//nastavenie premennych pre horny 
			Xmax = e2[0].x();

			y = e1[0].y();
			Ymax = e1[1].y();
			//qDebug() << "pre dolny " << Xmin << Xmax;
		}

		else {
			QVector<QPointF> e3(2), e4(2);
			float w3, w4;

			e1[0] = body[0];														//nehorizontalne hrany 
			e1[1] = P;

			e2[0] = body[0];
			e2[1] = body[1];

			e3[0] = P;
			e3[1] = body[2];

			e4[0] = body[1];
			e4[1] = body[2];


			m = (e3[1].y() - e3[0].y()) / (double)(e3[1].x() - e3[0].x());			//nastavenie premennych pre dolny 
			w3 = 1.0 / m;
			m = (e4[1].y() - e4[0].y()) / (double)(e4[1].x() - e4[0].x());
			w4 = 1.0 / m;
			Xmin = e3[0].x();
			Xmax = e4[0].x();
			y = e3[0].y();
			Ymax = e3[1].y();

			int dx = Xmax - Xmin;
			while (y < Ymax) {														//kreslenie dolneho	
				for (i = 1; i <= dx; i++) {
					
					if (algovypln == 0) {				//nearest neighbor
						setPixel((int)Xmin + i, y, nearest(int(Xmin) + i, y, body, farby));
					}
					else if (algovypln == 1) {			//barycentric
						setPixel((int)Xmin + i, y, bary(int(Xmin) + i, y, body, farby));
					}
				}
				Xmin += w3;
				Xmax += w4;
				y++;
				dx = Xmax - Xmin;
			}
			Xmin = e1[0].x();														//nastavenie premennych pre horny 
			Xmax = e2[0].x();
			y = e1[0].y();
			Ymax = e1[1].y();
		}
		m = (e1[1].y() - e1[0].y()) / (double)(e1[1].x() - e1[0].x());				//kreslenie horneho
		w1 = 1.0 / m;
		m = (e2[1].y() - e2[0].y()) / (double)(e2[1].x() - e2[0].x());
		w2 = 1.0 / m;



		int dx = Xmax - Xmin;
		while (y < Ymax) {														//kreslenie dolneho	
			for (i = 1; i <= dx; i++) {
				if (algovypln == 0) {				//nearest neighbor
					setPixel((int)Xmin + i, y, nearest(int(Xmin) + i, y, body, farby));
				}
				else if (algovypln == 1) {			//barycentric
					setPixel((int)Xmin + i, y, bary(int(Xmin) + i, y, body, farby));
				}
			}
			Xmin += w1;
			Xmax += w2;
			y++;
			dx = Xmax - Xmin;
		}
	}

	if (ibajeden) {
		m = (e1[1].y() - e1[0].y()) / (double)(e1[1].x() - e1[0].x());
		w1 = 1.0 / m;
		m = (e2[1].y() - e2[0].y()) / (double)(e2[1].x() - e2[0].x());
		w2 = 1.0 / m;

		int dx = Xmax - Xmin;
		while (y < Ymax) {
			for (i = 1; i <= dx; i++) {
				if (algovypln == 0) {				//nearest neighbor
					setPixel((int)Xmin + i, y, nearest(int(Xmin) + i, y, body, farby));
				}
				else if (algovypln == 1) {			//barycentric
					setPixel((int)Xmin + i, y, bary(int(Xmin) + i, y, body, farby));

				}
			}
			Xmin += w1;
			Xmax += w2;
			y++;
			dx = Xmax - Xmin;
		}
	}
}


QColor ViewerWidget::nearest(int x, int y, QList<QPointF> body, QList<QColor> farby) {
	int i, j;
	QColor c1(255, 0, 0), c2(0, 255, 0), c3(0, 0, 255);
	c1 = farby[0]; c2 = farby[1]; c3 = farby[2];
	float d0, d1, d2;
	d0 = sqrt((x - body[0].x()) * (x - body[0].x()) + (y - body[0].y()) * (y - body[0].y()));
	d1 = sqrt((x - body[1].x()) * (x - body[1].x()) + (y - body[1].y()) * (y - body[1].y()));
	d2 = sqrt((x - body[2].x()) * (x - body[2].x()) + (y - body[2].y()) * (y - body[2].y()));

	if (d0 < d1 && d0 <= d2) {
		return c1;
	}
	else if (d1 < d0 && d1 <= d2) {
		return c2;
	}
	else if (d2 < d0 && d2 <= d1) {
		return c3;
	}
	else {
		return QColor(0, 0, 0);
	}
}

QColor ViewerWidget::bary(int x, int y, QList<QPointF> body, QList<QColor> farby) {
	float A, A0, A1, L0, L1, L2;
	float r1, r2, r3, g1, g2, g3, b1, b2, b3;
	r1 = farby[0].red(); r2 = farby[1].red(); r3 = farby[2].red();
	g1 = farby[0].green(); g2 = farby[1].green(); g3 = farby[2].green();
	b1 = farby[0].blue(); b2 = farby[1].blue(); b3 = farby[2].blue();
	QColor bary;
	A = qAbs((body[1].x() - body[0].x()) * (body[2].y() - body[0].y()) - (body[1].y() - body[0].y()) * (body[2].x() - body[0].x())) / 2.0;
	A0 = qAbs((body[1].x() - x) * (body[2].y() - y) - (body[1].y() - y) * (body[2].x() - x)) / 2.0;
	A1 = qAbs((body[0].x() - x) * (body[2].y() - y) - (body[0].y() - y) * (body[2].x() - x)) / 2.0;
	L0 = A0 / (double)A;
	L1 = A1 / (double)A;
	L2 = 1.0 - L1 - L0;
	bary.setRed(L0 * r1 + L1 * r2 + L2 * r3);
	bary.setGreen(L0 * g1 + L1 * g2 + L2 * g3);
	bary.setBlue(L0 * b1 + L1 * b2 + L2 * b3);
	return bary;
}


void ViewerWidget::kresliHedron(QList<QPointF> naVykreslenie, QList<int> nevykresluj, QList<QColor> farby, int algovypln) {
	img->fill(Qt::white);
	qDebug() << naVykreslenie.size() << farby.size();
	update();
	int i, k;
	int w = img->width();
	int h = img->height();
	bool nekresli = false;
	QList<QPointF> W, V;
	QList<QColor> farby3;
	for (i = 0; i < naVykreslenie.size(); i += 3) {
		nekresli = false;
		for (k = 0; k < nevykresluj.size(); k++) {
			if (i/3 == nevykresluj[k]) {
				nekresli = true;
			}
		}
		if (!nekresli) {
			farby3.clear();
			farby3.append(farby[i]);
			farby3.append(farby[i+1]);
			farby3.append(farby[i+2]);
			V.clear();
			V.append(naVykreslenie[i]);
			V.append(naVykreslenie[i + 1]);
			V.append(naVykreslenie[i + 2]);
			QPointF S, P, A, B;
			int Xmin[4] = { 1, 1, -w + 1, -h + 1 };
			int xmin = 0, j = 0, i = 0;
			for (j = 0; j < 4; j++) {
				xmin = Xmin[j];
				if (V.size() > 0)
					S = V[V.size() - 1];
				for (i = 0; i < V.size(); i++) {
					if (V[i].x() >= xmin) {
						if (S.x() >= xmin) {
							W.push_back(V[i]);
						}
						else {
							P.setX(xmin);
							P.setY(S.y() + (xmin - S.x()) * (V[i].y() - S.y()) / (V[i].x() - S.x()));
							W.push_back(P);
							W.push_back(V[i]);
						}
					}
					else {
						if (S.x() >= xmin) {
							P.setX(xmin);
							P.setY(S.y() + (xmin - S.x()) * (V[i].y() - S.y()) / (V[i].x() - S.x()));
							W.push_back(P);
						}
					}
					S = V[i];
				}
				V.clear();
				for (i = 0; i < W.size(); i++) {
					P.setX(W[i].y());
					P.setY(-W[i].x());
					V.push_back(P);
				}
				W.clear();
			}
			for (i = 0; i < V.size(); i++) {
				qDebug() << V.size();
				A.setX(V[i].x());
				A.setY(V[i].y());
				B.setX(V[(i + 1) % (V.size())].x());
				B.setY(V[(i + 1) % (V.size())].y());
				if (!isnan(A.x()) && !isnan(B.x()) && !isnan(A.y()) && !isnan(B.y()))
					usecka_DDA(A, B, Qt::black);
			}
			if (V.size() == 3)
				scanLineTri(V, Qt::black, algovypln, farby3);
		}
	}
	update();
}

//Slots
void ViewerWidget::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);
	QRect area = event->rect();
	painter.drawImage(area, *img, area);
}
