#pragma once

#include <QtWidgets/QMainWindow>
#include <QtWidgets>
#include "ui_ImageViewer.h"
#include "ViewerWidget.h"
#include "NewImageDialog.h"
#include "Objekt.h"

class ImageViewer : public QMainWindow
{
	Q_OBJECT

public:
	ImageViewer(QWidget* parent = Q_NULLPTR);

private:
	Ui::ImageViewerClass* ui;
	NewImageDialog* newImgDialog;

	QSettings settings;
	QMessageBox msgBox;

	//ViewerWidget functions
	ViewerWidget* getViewerWidget(int tabId);
	ViewerWidget* getCurrentViewerWidget();

	//Event filters
	bool eventFilter(QObject* obj, QEvent* event);

	//ViewerWidget Events
	bool ViewerWidgetEventFilter(QObject* obj, QEvent* event);
	void ViewerWidgetMouseButtonPress(ViewerWidget* w, QEvent* event);
	void ViewerWidgetMouseButtonRelease(ViewerWidget* w, QEvent* event);
	void ViewerWidgetMouseMove(ViewerWidget* w, QEvent* event);
	void ViewerWidgetLeave(ViewerWidget* w, QEvent* event);
	void ViewerWidgetEnter(ViewerWidget* w, QEvent* event);
	void ViewerWidgetWheel(ViewerWidget* w, QEvent* event);

	//ImageViewer Events
	void closeEvent(QCloseEvent* event);

	//Image functions
	void openNewTabForImg(ViewerWidget* vW);
	bool openImage(QString filename);
	bool saveImage(QString filename);
	void clearImage();

	//Inline functions
	inline bool isImgOpened() { return ui->tabWidget->count() == 0 ? false : true; }

	Hedron octa;
	premietanie kamera;
	QList<QPointF> naVykreslenie;
	QList<QColor> farby;
	QList<int> nevykresluj;
	QList<double> zSur;
	void prepocitajSuradnice();
	void suradniceRovnobezne();
	void suradniceStredove();
	void normalaCalc();
	void farbyCalc();


private slots:
	//Tabs slots
	void on_tabWidget_tabCloseRequested(int tabId);
	void on_actionRename_triggered();

	//Image slots
	void on_actionNew_triggered();
	void newImageAccepted();
	void on_actionOpen_triggered();
	void on_actionSave_as_triggered();
	void on_actionClear_triggered();

	// octahedron slots
	void on_generuj_clicked();
	void on_rozdel_clicked();
	void on_imp_clicked();
	void on_exp_clicked();
	void on_zoom_valueChanged(int i);
	void on_azimut_valueChanged(int i);
	void on_zenit_valueChanged(int i);
	void on_horizontalSlider_valueChanged(int i);
	void on_verticalSlider_valueChanged(int i);
	void on_comboBox_currentIndexChanged(int index);
	void on_spinFar_valueChanged(int i);
	void on_spinNear_valueChanged(int i);
	void on_spinBox_3_valueChanged(int i);
	void on_pushButton_clicked();
	void on_algo_currentIndexChanged(int i);
};
