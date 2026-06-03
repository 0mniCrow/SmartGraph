#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QKeyEvent>
#include <QMessageBox>
#include "vectorgraph.h"
#include "listgraph.h"
#include "Graph_algorithms.h"
#include "matrixmodel.h"
#include "touchform.h"
//___________________Visual graph______________________
#include "gviewport.h"
#include "gview_tableVertexView.h"
//______________________End____________________________
//#include "visualisationgraphform.h"
#include "imagecropwindow.h"
#include "xmlparser.h"
#include "gview_localization_controller.h"
#include "widgets/gview_timeline_tool.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

using std::vector;
using std::string;

#define DEVELOPER_MODE
#define MINIMAL_BG_SIZE 1000
#define TEST_TIMELINE_TICKS 12
#define TIMELINE_JUMP 10

class MainWindow : public QMainWindow
{
    Q_OBJECT
protected:
    virtual void closeEvent         (QCloseEvent*  cl_event) override;
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
public slots:
    void setProgressBar(int val, int max);
private slots:
    void execute();
    //_____________Visual graph_______________
    void AddObject();
    void RemoveObject();
    void CreateEdge();
    void RemoveEdge();
    void ChangeSize(int radius);
    void updateInfo(QString info);
    void fixEdges(bool status);
    void setBG();
    void SaveProject();
    void LoadProject();
    void SaveObjectList();
    void LoadLanguageFile();
    void HideShowUI();
    void setZoomMode(int state);
    void setDragMode(int state);
    void changeLanguage(int index);

    //____________Timeline control____________;
    void HideShowTimeLine();
    void tlPlay();
    void tlPause();
    void tlStop();
    void tlStepBack();
    void tlJumpBack();
    void tlStepForward();
    void tlJumpForward();
    void tlChangeScale(int index);

    //_______________End______________________
private:
    TouchForm * touchform;
//  ImageCropWindow _crop_window_;
//  VisualisationGraphForm *vis_form;
    MatrixModel * model;
    GviewLangControl _translation_control_;
    GViewTimeTool _timeline_tool_;
    GViewTimeInterface* _timeline_interface_;
    //____________Visual graph_______________
    GViewScene* _scene_;
    GViewPort* _view_;
    VertexModel* _model_;
    VertexList* _vert_list_;
    QString _bg_filename_;
    //______________End________________________
    void keyPressEvent(QKeyEvent* pe) override;
    void initiateGraphicsView();
    void initiateTimelineTool();
    void LoadBGFromFile(const QString& addr);
    void loadTranslatableMessages();
    void loadTranslatableWindows();
    void updateTimeTool();
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
