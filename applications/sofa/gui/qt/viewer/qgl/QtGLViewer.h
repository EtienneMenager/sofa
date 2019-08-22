/******************************************************************************
*       SOFA, Simulation Open-Framework Architecture, development version     *
*                (c) 2006-2019 INRIA, USTL, UJF, CNRS, MGH                    *
*                                                                             *
* This program is free software; you can redistribute it and/or modify it     *
* under the terms of the GNU General Public License as published by the Free  *
* Software Foundation; either version 2 of the License, or (at your option)   *
* any later version.                                                          *
*                                                                             *
* This program is distributed in the hope that it will be useful, but WITHOUT *
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       *
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for    *
* more details.                                                               *
*                                                                             *
* You should have received a copy of the GNU General Public License along     *
* with this program. If not, see <http://www.gnu.org/licenses/>.              *
*******************************************************************************
* Authors: The SOFA Team and external contributors (see Authors.txt)          *
*                                                                             *
* Contact information: contact@sofa-framework.org                             *
******************************************************************************/
#ifndef SOFA_GUI_QGLVIEWER_QTVIEWER_H
#define SOFA_GUI_QGLVIEWER_QTVIEWER_H

#include <sofa/gui/qt/gl/SofaGUIQtOpenGL.h>

#include <sofa/helper/system/gl.h>
#include <qgl.h>
#include <qtimer.h>
#include <cmath>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <fstream>

#include <sofa/gui/qt/viewer/OglModelPolicy.h>
#include <sofa/gui/ViewerFactory.h>
#include <sofa/defaulttype/Vec.h>
#include <sofa/defaulttype/Quat.h>
#include <sofa/helper/gl/Transformation.h>
#include <sofa/helper/gl/Trackball.h>
#include <sofa/helper/gl/Texture.h>
#include <sofa/helper/system/thread/CTime.h>
#include <SofaSimulationCommon/xml/Element.h>



#include <QGLViewer/qglviewer.h>
#include <QGLViewer/manipulatedFrame.h>

#define TRACKING_MOUSE

namespace sofa
{

namespace gui
{

namespace qt
{

namespace viewer
{

namespace qgl
{

class SOFA_SOFAGUIQTOPENGL_API QtGLViewer :public QGLViewer,   public sofa::gui::qt::viewer::OglModelSofaViewer
{
    typedef defaulttype::Vector3::value_type Real;
    Q_OBJECT
private:

#ifdef TRACKING_MOUSE
    bool m_grabActived;
#endif

    QTimer* timerAnimate;
    int				_W, _H;
    int				_clearBuffer;
    bool			_lightModelTwoSides;
    float			_lightPosition[4];


    double lastProjectionMatrix[16];
    double lastModelviewMatrix[16];

    GLUquadricObj*	_arrow;
    GLUquadricObj*	_tube;
    GLUquadricObj*	_sphere;
    GLUquadricObj*	_disk;
    GLuint _numOBJmodels;
    GLuint _materialMode;
    GLboolean _facetNormal;

    int _renderingMode;

    helper::system::thread::ctime_t _beginTime;


    bool _waitForRender;


public:

    static QtGLViewer* create(QtGLViewer*, sofa::gui::BaseViewerArgument& arg)
    {
        BaseViewerArgument* pArg = &arg;
        ViewerQtArgument* viewerArg = dynamic_cast<ViewerQtArgument*>(pArg);
        return viewerArg ?
                new QtGLViewer(viewerArg->getParentWidget(), viewerArg->getName().c_str(), viewerArg->getNbMSAASamples() ) :
                new QtGLViewer(nullptr, pArg->getName().c_str(), pArg->getNbMSAASamples() )
                ;
    }

    static const char* viewerName()  { return "QGLViewer"; }

    static const char* acceleratedName()  { return "&QGLViewer"; }

    virtual void drawColourPicking (ColourPickingVisitor::ColourCode code) override;

    QtGLViewer( QWidget* parent, const char* name="", const unsigned int nbMSAASamples = 1 );
    ~QtGLViewer() override;

    QWidget* getQWidget() override { return this; }

protected:
     static QGLFormat setupGLFormat(const unsigned int nbMSAASamples = 1);

    //     void calcProjection();
    void init() override;
    /// Overloaded from QGLViewer to render the scene
    void draw() override;
    /// Overloaded from SofaViewer
    virtual void viewAll() override;
    void resizeGL( int w, int h ) override;

public:

    //void			reshape(int width, int height);
    int getWidth() override
    {
        return _W;
    }
    int getHeight() override
    {
        return _H;
    }
    bool ready() override {return !_waitForRender;}
    void wait() override {_waitForRender = true;}

    void	UpdateOBJ(void);

    void moveRayPickInteractor(int eventX, int eventY) override;

    void setCameraMode(core::visual::VisualParams::CameraType mode) override;

    QString helpString() const override;


private:

    void	InitGFX(void);
    void	PrintString(void* font, char* string);
    void	Display3DText(float x, float y, float z, char* string);
    void	DrawAxis(double xpos, double ypos, double zpos, double arrowSize);
    void	DrawBox(Real* minBBox, Real* maxBBox, Real r=0.0);
    void	DrawXYPlane(double zo, double xmin, double xmax, double ymin, double ymax, double step);
    void	DrawYZPlane(double xo, double ymin, double ymax, double zmin, double zmax, double step);
    void	DrawXZPlane(double yo, double xmin, double xmax, double zmin, double zmax, double step);
    void	CreateOBJmodelDisplayList(int material_mode);
    //int     loadBMP(char *filename, TextureImage *texture);
    //void	LoadGLTexture(char *Filename);
    void	DisplayOBJs();
    void	DisplayMenu(void);
    void        MakeStencilMask();

    //int		handle(int event);	// required by FLTK

protected:
    //virtual bool event ( QEvent * e );

    virtual void	drawScene() override;
    virtual void	DrawLogo(void);


    void keyPressEvent ( QKeyEvent * e ) override;
    void keyReleaseEvent ( QKeyEvent * e ) override;
    void mousePressEvent ( QMouseEvent * e ) override;
    void mouseReleaseEvent ( QMouseEvent * e ) override;
    void mouseMoveEvent ( QMouseEvent * e ) override;
    void wheelEvent(QWheelEvent* e) override;
    bool mouseEvent( QMouseEvent * e ) override;



public slots:
    void resetView() override;
    void saveView() override;
    void setSizeW(int) override;
    void setSizeH(int) override;

    virtual void getView(defaulttype::Vec3d& pos, defaulttype::Quat& ori) const override;
    virtual void setView(const defaulttype::Vec3d& pos, const defaulttype::Quat &ori) override;
    virtual void captureEvent() override { SofaViewer::captureEvent(); }
    void fitObjectBBox(sofa::core::objectmodel::BaseObject* object) override
    {
        if( object->f_bbox.getValue().isValid() && !object->f_bbox.getValue().isFlat() )
            this->camera()->fitBoundingBox(
                ::qglviewer::Vec(object->f_bbox.getValue().minBBox()),
                ::qglviewer::Vec(object->f_bbox.getValue().maxBBox())
            );
        else
        {
            if(object->getContext()->f_bbox.getValue().isValid() && !object->getContext()->f_bbox.getValue().isFlat()  )
            {
                this->camera()->fitBoundingBox(
                    ::qglviewer::Vec(object->getContext()->f_bbox.getValue().minBBox()),
                    ::qglviewer::Vec(object->getContext()->f_bbox.getValue().maxBBox())
                );
            }
        }
        this->update();
    }

    void fitNodeBBox(sofa::core::objectmodel::BaseNode* node) override
    {
        if( node->f_bbox.getValue().isValid() && !node->f_bbox.getValue().isFlat() )
            this->camera()->fitBoundingBox(
                ::qglviewer::Vec(node->f_bbox.getValue().minBBox()),
                ::qglviewer::Vec(node->f_bbox.getValue().maxBBox())
            );

        this->update();

    }

signals:
    void redrawn() override;
    void resizeW( int ) override;
    void resizeH( int ) override;
    void quit();
};

} // namespace qgl

} // namespace viewer

} //namespace qt

} // namespace gui

} // namespace sofa

#endif


