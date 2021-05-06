#pragma once
#include <QtWidgets>
class ViewerWidget :public QWidget {
	Q_OBJECT
private:
	QString name = "";
	QSize areaSize = QSize(0, 0);
	QImage* img = nullptr;
	QPainter* painter = nullptr;
	QRgb* data = nullptr;

	bool freeDrawActivated = false;
	QPoint freeDrawBegin = QPoint(0, 0);

public:
	ViewerWidget(QString viewerName, QSize imgSize, QWidget* parent = Q_NULLPTR);
	~ViewerWidget();
	void resizeWidget(QSize size);

	//Image functions
	bool setImage(const QImage& inputImg);
	QImage* getImage() { return img; };
	bool isEmpty();
	

	//Draw functions
	void freeDraw(QPoint end, QPen pen);
	void setFreeDrawBegin(QPoint begin) { freeDrawBegin = begin; }
	QPoint getFreeDrawBegin() { return freeDrawBegin; }
	void setFreeDrawActivated(bool state) { freeDrawActivated = state; }
	bool getFreeDrawActivated() { return freeDrawActivated; }
	void usecka_DDA(QPointF A, QPointF B, QColor color);
	void kresliHedron(QList<QPointF> naVykreslenie, QList<int> nevykresluj);

	//Data functions
	QRgb* getData() { return data; }
	void setPixel(int x, int y, const QColor& color);
	void setPixel(int x, int y, unsigned char r, unsigned char g, unsigned char b);
	bool isInside(int x, int y) { return (x >= 0 && y >= 0 && x < img->width() && y < img->height()) ? true : false; }
	void clear(QColor color = Qt::white);

	//Get/Set functions
	
	void setDataPtr() { data = reinterpret_cast<QRgb*>(img->bits()); }



	//Get/Set functions
	QString getName() { return name; }
	void setName(QString newName) { name = newName; }

	void setPainter() { painter = new QPainter(img); }

	int getImgWidth() { return img->width(); };
	int getImgHeight() { return img->height(); };

	void clear();

public slots:
	void paintEvent(QPaintEvent* event) Q_DECL_OVERRIDE;
};