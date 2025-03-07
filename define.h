#ifndef DEFINE_H
#define DEFINE_H

#include <QRect>
#include <QPoint>

#define APP_NAME            "TegeViewer"
#define APP_VERSION         "1.20"

#define MAX_CAPTURE         5

#define STARTUP_WIDTH       1000
#define STARTUP_HEIGHT      700

#define SELTYPE_RECT	    0
#define SELTYPE_FIX		    1
#define SELTYPE_HORZ	    2
#define SELTYPE_VERT	    3
#define SELTYPE_FULL	    4
#define SELTYPE_NULL	    9

#define NUM_COLOR           3
#define COLOR_INDEX_R       0
#define COLOR_INDEX_G       1
#define COLOR_INDEX_B       2

#define ZOOMLIMIT_MAX       80
#define ZOOMLIMIT_MIN       -50

#define GRAPH_HEADER        35      // radiobutton, checkbox
#define GRAPH_LEFT          50      // Y Axis
#define GRAPH_RIGHT         10      // margin
#define GRAPH_FOOTER        40      // X Axis

#define HIST_NUM            256     // 8bit only

#define GRAPHTYPE_LEVEL     0
#define GRAPHTYPE_HISTO 	1
#define GRAPH_SERIES_MAX    1023

#define ANADIR_HORZ         0
#define ANADIR_VERT         1

#define TAB                 "\t"
#define LF                  "\r"

struct analysisData {
    int maxVal[NUM_COLOR];
    int minVal[NUM_COLOR];
    QPoint maxValPos[NUM_COLOR];
    QPoint minValPos[NUM_COLOR];
    int diffVal[NUM_COLOR];
    double avrg[NUM_COLOR];
    double sd[NUM_COLOR];
    double variance[NUM_COLOR];
    double sn[NUM_COLOR];
    int pixels;
};
#endif // DEFINE_H
