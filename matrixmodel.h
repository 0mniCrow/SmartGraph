#ifndef MATRIXMODEL_H
#define MATRIXMODEL_H
#include <QAbstractTableModel>
#include <QColor>
#include <QTimer>
#include <vector>
#include <string>

#define DEFAULT_ROW_COUNT 6
#define DEFAULT_COL_COUNT 6
#define DEFAULT_TIMER 1200
#define INTEGER_MATRIX
//#define CHAR_MATRIX
#define COLOR_DEPENDENT_INT

using std::vector;
using std::string;

struct PlayAction
{
    enum PlayActionsType{ PAct_Safe = 0, PAct_Warn = 1, PAct_Err = 2 };
    int row;
    int column;
    string value;
    PlayActionsType actionType;
    bool to_change;
    int val_to_change;
};

class MatrixModel:public QAbstractTableModel
{
private:
    Q_OBJECT
#ifdef INTEGER_MATRIX
    vector<vector<int>> _visual_data_;
#elif defined(CHAR_MATRIX)
    vector<vector<char>> _visual_data_;
#endif
    vector<PlayAction> _actions_;
    vector<PlayAction>::iterator _cur_action_,_next_action_;
    bool _play_flag_;
    bool chBorderSize(int row, int col) const;
public:
    MatrixModel(QObject* tata = nullptr);
    MatrixModel(int rows, int cols, QObject* tata = nullptr);

#ifdef INTEGER_MATRIX
    MatrixModel(const vector<vector<int>>& model, QObject* tata = nullptr);
#elif defined(CHAR_MATRIX)
    MatrixModel(const vector<vector<char>>& model, QObject* tata = nullptr);
#endif

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    QVariant data(const QModelIndex& index,int role = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;
    bool insertRows(int row, int count, const QModelIndex& parent = QModelIndex()) override;
    bool insertColumns(int column, int count, const QModelIndex& parent = QModelIndex()) override;
    bool removeRows(int row, int count, const QModelIndex& parent = QModelIndex()) override;
    bool removeColumns(int column, int count, const QModelIndex& parent = QModelIndex()) override;

#ifdef INTEGER_MATRIX
    bool setData(const vector<vector<int>>& data);
#elif defined(CHAR_MATRIX)
    bool setData(const vector<vector<char>>& data);
#endif

    bool setActions(const vector<PlayAction>& actions);
private slots:
    bool play();
public slots:
    void startActions();
    void changePlayFlag(bool play_flag);
signals:
    void updateBar(int cur_action, int whole_actions);


};

#endif // MATRIXMODEL_H
