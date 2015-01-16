#ifndef DATAWINDOW_H
#define DATAWINDOW_H

#include "eztable.h"
#include "variable.h"
#include <QtGui>
#include <QScrollArea>

class DataGroup;
class DataPlank;

/**
 *  \typedef DataMap
 *
 *  A \c DataMap is a \c QHash mapping the name of a \c DataPlank
 *  to the pointer to the \c DataPlank in the tree widget displayed in
 *  the data window.  Maps of this type allow updating a \c DataPlank
 *  rapidly starting with the name of the \c DataPlank.
 */
typedef QHash<QString, DataPlank*> DataMap;

/**
 *  \class DataPlank
 *
 *  \brief The \c DataPlank class define one plank in the \c QTreeWidget
 *         corresponding to one variable, array element or class member.
 *
 *  \c DataPlank is derived from the \c QTreeWidgetItem class which is
 *  the type for the individual planks in a \c QTreeWidget.  In a tree
 *  widget some planks can be expanded into multiple sub-planks and others
 *  cannot be expanded.  In the data tree the simple data planks like
 *  char, short, double, etc can't be expanded while complex data planks
 *  like arrays, classes and structs can be expanded into sub-planks.
 *
 *  Internally a \c DataPlank with simple data holds it in binary form
 *  in an anonymous union of size 8 bytes.  The data received from
 *  \c gdb is in hexadecimal format and is stored in an unsigned
 *  portion of the union of the proper *  size with all the remaining
 *  bits set to 0.  The union has all the simple types in it and, 
 *  depending on the format, the proper member of the union is used
 *  to retrieve the formatted value of the \c DataPlank.
 *
 *  In addition to the anonymous union a \c DataPlank holds the name
 *  of the data plank.  For an array with name "stuff", when expanded
 *  to show 3 array elements with indexes 0, 1 and 2, the child nodes
 *  would have names "stuff[0]", "stuff[1]" and "stuff[2]".  Likewise
 *  if the array is an array of class planks, the child nodes of "stuff[0]"
 *  might be named "stuff[0].name" and "stuff[0].address".  This
 *  strategy simplifies the expansion of nodes to arbitrary depth.
 *
 *  There are 4 categories of data in the data window: globals, locals,
 *  parameters and user-defined.  Each of these categories has an
 *  associated DataMap and plank \c DataPlank contains a pointer to its
 *  \c DataMap.
 */
class DataTree;

class IndicatorButton: public QPushButton
{
public:
    IndicatorButton(QWidget *parent=0);
    EZ::State *state;
    void paintEvent(QPaintEvent *ev);
    void mouseReleaseEvent(QMouseEvent *ev);
};

class DataPlank : public EZPlank
{
public:
    DataPlank(QWidget *parent=0);
    QString value();
    QString name;       ///< Full name of the \c DataPlank
    QString type;       ///< Type as reported by \c gdb
    QString basicType;  ///< Type without [] or *
    QString address;    ///< For user-defined variables
    QString format;     ///< How the \c DataPlank should look
    QVector<Limits> dimensions;  ///< Array limits
    bool isSimple;      ///< Is the data simple, like char or int
    bool isFortran;     ///< Was this a fortran variable
    bool isFinal;
    int size;           ///< Number of bytes of data
    int root;
    int treeLevel;
    int kidCount;
    int frame;
    IndicatorButton *indicator;
    DataTree *tree;
    DataPlank *parent;
    QVector<DataPlank *> kids;
    void collapse();
    DataPlank *addPlank(QString name);
    DataMap parameterMap;
    DataMap localMap;
    EZ::State state;
    void removeSubTree();
    QString valueFromGdb();
    DataMap *map;       ///< Map containing this \c DataPlank
    QString stringValue;       ///< Value printed by \c gdb
    QStringList stringValues;      ///< Value printed by \c gdb
    AllTypes a;         ///< Union of all basic types
    AllTypesArray *values;         ///< Union of all basic types
    bool hidden;
    void addChild(DataPlank *plank) {
        plank->treeLevel = treeLevel+1;
        kids.append(plank);
    }
    void removeChild(DataPlank *plank);
    int childCount() {
        return kids.size();
    }
    void deactivate();
    void reactivate();
    void sortChildren();
    void setName(QString s);
    void setType(QString s);
    void setValue(QString s);
    void setValues(QStringList strings);
    int maxLevel();
    int countPlanks();
};

/**
 *  \class DataTree
 *
 *  \c DataTree is derived from the Qt \c QTreeWidget class which
 *  provides a tree view for displaying data.
 *
 *  There is one \c DataTree in the data window and it contains
 *  4 top-level planks: \c globals, \c locals, \c parameters and
 *  \c userDefined.  Each of these \c DataPlank serves as the root
 *  of their associated data planks.
 *
 *  Global data is any variable defined outside a function (or class) or defined
 *  as \c static inside a function (or class).  These planks are discovered
 *  by inspecting the object files for a program before linking to avoid
 *  cluttering the screen with a large number of global data associated with
 *  library functions.
 *
 *  Local data means variable defined without the keyword \c static inside
 *  a function.  These are provided by \c gdb at each point that the debugger
 *  stops and reports the state of the debugged program.
 *
 *  Parameters are the data planks enclosed in parentheses 
 */
class DataTree: public EZTable
{
    Q_OBJECT

public:
    DataTree(QWidget *parent=0);
    ~DataTree();
    void contextMenuEvent(QContextMenuEvent *event);
    DataPlank *all;
    DataPlank *globals;
    int rows;
    int columns;
    int levels;
    int endName;
    int nextRow;
    int fontWidth;
    int fontHeight;
    void reorder(DataPlank *plank);
    void redisplay ( DataPlank *p, EZ::Color highlight );
    DataPlank *addDataPlank(DataPlank *parent, int level, DataMap *map,
                            QString n, QString t);
    QList<Limits> dimensions;
    QList<DataPlank*> kids;
    void buildTree(DataPlank *plank);
    void setPlankCount(int n);
    void expandDataPlank(DataPlank*);
    void collapseDataPlank(DataPlank*);

public slots:
    void receiveBackTrace(QStringList);
    void editUserVariable();
    void deleteUserVariable();
    void setDecimal();
    void setFloatingPoint();
    void setBool();
    void setBinary();
    void setBinaryFP();
    void setFields();
    void setCharacter();
    void setUnsignedDecimal();
    void setHexadecimal();

private:

signals:
    void requestParameters ( DataPlank *p, int frame );
    void requestLocals ( DataPlank *p, int frame );
};

class DataWindow: public QFrame
{
    Q_OBJECT

public:
    DataWindow(QWidget *parent = 0);
    void setFontHeightAndWidth(int height, int width);
    void request(DataPlank *plank);
    int treeLevel;
    QVBoxLayout *layout;
    int fontHeight;
    int fontWidth;
    int xScroll;
    int yScroll;
    QScrollArea *scrollArea;
    void receiveVars(DataMap *group, VariableDefinitionMap &vars);
    void saveScroll();
    void restoreScroll();

public slots:
    void receiveVariableDefinition(QStringList);
    void resetData();
    void receiveClasses(QHash<QString, ClassDefinition> c);
    void receiveVar(DataPlank *p, QString name, QStringList values);
    void receiveGlobals(VariableDefinitionMap vars);
    void receiveLocals(DataPlank *p,VariableDefinitionMap vars);
    void receiveParameters(DataPlank *p,VariableDefinitionMap vars);

private:
    QSize sizeHint() const;

    signals:
    void requestVar(DataPlank*,QString,QString,QString,QString,int,int);
};

#endif
