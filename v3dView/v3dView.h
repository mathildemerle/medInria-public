// /////////////////////////////////////////////////////////////////
// Generated by dtkPluginGenerator
// /////////////////////////////////////////////////////////////////

#ifndef V3DVIEW_H
#define V3DVIEW_H

#include <dtkCore/dtkAbstractData.h>
#include <dtkCore/dtkAbstractView.h>

#include <medAbstractView.h>

#include "v3dViewPluginExport.h"

class QMouseEvent;

class v3dViewPrivate;
class v3dViewObserver;
class vtkImageView;
class vtkImageView2D;
class vtkImageView3D;
class vtkRenderer;
class vtkRenderWindowInteractor;

/**
 * @class v3dView
 * @brief Concrete implementation of a medAbstractView using vtkinria3d's vtkImageView system.
 * v3dView stands for vtkinria3dView. It embeds in a QVTKWidget a visualization system for 2D
 * and 3D images.
 * Multiple overlays are supported: in 2D, overlays are layers of images displayed on top of
 * each others. Layer N is always drawn on top of layer N-1. Opacity, LUT and visibility can
 * be set per layer. In 3D, only VR is supported.
 **/

class V3DVIEWPLUGIN_EXPORT v3dView : public medAbstractView
{
    Q_OBJECT

public:
             v3dView(void);
    virtual ~v3dView(void);

    virtual QString description(void) const;
    virtual QString identifier(void) const;

    static bool registered(void);

public:
    // inherited from medAbstractView
    void reset(void);
    void clear(void);
    void update(void);

    void *view(void);

    /**
     * Inputs the data to the vtkImageView2D/3D.
     * @param layer - specifies at which layer the image is inputed.
     * Layer N if always shown on top of layer N-1. By playing with
     * visibility and opacity, it is possible to show multiple images
     * on top of each others.
     */
    void setData(dtkAbstractData *data, int layer);

	void setSharedDataPointer(dtkSmartPointer<dtkAbstractData> data);

    /**
     * Inputs the data to the vtkImageView2D/3D instances.
     * Calling setData(data) will automatically position the data in
     * the next available layer. Example:
     * - first call  -> layer 0
     * - second call -> layer 1
     * ...
     * To set the data at a specific layer, call setData(data, layer).
     * To set the data to the first layer, call setData(data, 0).
     */
    void setData(dtkAbstractData *data);

    void *data (void);

    QWidget *receiverWidget(void);
    QWidget *widget(void);

    void close(void);

    // access method to internal members for v3dView**Interactor classes

    /**
     * Returns pointers to internal instances of vtkImageView2D/3D.
     */
    vtkImageView2D *view2d(void);
    vtkImageView3D *view3d(void);

    /**
     * Returns a pointer to the current view, being 2D or 3D.
     */
    vtkImageView *currentView(void);

    /**
     * Returns the vtkRenderWindowInteractor instance.
     */
    vtkRenderWindowInteractor *interactor(void);

    /**
     * Access methods to vtkRenderer members.
     */
    vtkRenderer *renderer2d(void);
    vtkRenderer *renderer3d(void);

    virtual QStringList getAvailableTransferFunctionPresets();
    virtual void getTransferFunctions( QList<double> & scalars,
				       QList<QColor> & colors );
    virtual void setTransferFunctions( QList<double> scalars,
				       QList<QColor > colors );
    virtual void setColorLookupTable( QList< double > scalars,
				      QList< QColor > colors );

    /**
     * Set/Get the visibility of the image at layer given by @param layer.
     */
    virtual bool visibility(int layer) const;

    /**
     * Set/Get the opacity of the image at layer given by @param layer.
     */
    virtual double opacity(int layer) const;

    /**
     * Returns the total number of layers of the vtkImageView* instance.
     */
    virtual int layerCount(void) const;

    /**
     * Removes an overlay given the layer id.
     */
    virtual void removeOverlay(int layer);
   // QString property(const QString& key, int layer) const;
   // using dtkAbstractObject::property;
    QString getLUT(int layer) const;
    QString getPreset(int layer) const;
public slots:
    // inherited from medAbstractView
    void onPositionChanged  (const  QVector3D &position);
    void onZoomChanged      (double zoom);
    void onPanChanged       (const  QVector2D &pan);
    void onWindowingChanged (double level, double window);
    void onCameraChanged    (const  QVector3D &position,
                             const  QVector3D &viewup,
                             const  QVector3D &focal,
                             double parallelScale);

    void onVisibilityChanged(bool   visible, int layer);
    void onOpacityChanged   (double opacity, int layer);

public slots:
    void play          (bool value);
    void onPropertySet         (const QString &key, const QString &value);
    void onOrientationPropertySet           (const QString &value);
    void on3DModePropertySet                (const QString &value);
    void onRendererPropertySet              (const QString &value);
    void onUseLODPropertySet                (const QString &value);
    void onPresetPropertySet                (const QString &value);
    void onShowScalarBarPropertySet         (const QString &value);
    void onLookupTablePropertySet           (const QString &value);
    void onShowAxisPropertySet              (const QString &value);
    void onShowRulerPropertySet             (const QString &value);
    void onShowAnnotationsPropertySet       (const QString &value);
    void onMouseInteractionPropertySet      (const QString &value);
    void onCroppingPropertySet              (const QString &value);
    void onDaddyPropertySet                 (const QString &value);
    void onPositionLinkedPropertySet        (const QString &value);
    void onWindowingLinkedPropertySet       (const QString &value);

public slots:
    void onMousePressEvent                  (QMouseEvent *event);
    void onZSliderValueChanged              (int value);
    void onDimensionBoxChanged              (const QString &value);
    void onMetaDataSet         (const QString &key, const QString &value);

public slots: // Menu interface
    void onMenuAxialTriggered               (void);
    void onMenuSagittalTriggered            (void);
    void onMenuCoronalTriggered             (void);
    void onMenu3DVRTriggered                (void);
    void onMenu3DMaxIPTriggered             (void);
    void onMenu3DMinIPTriggered             (void);
    void onMenu3DMPRTriggered               (void);
    void onMenu3DOffTriggered               (void);
    void onMenuVRGPUTriggered               (void);
    void onMenuVRRayCastAndTextureTriggered (void);
    void onMenuVRRayCastTriggered           (void);
    void onMenuVRDefaultTriggered           (void);
    void onMenu3DLODTriggered               (void);
    void onMenuZoomTriggered                (void);
    void onMenuWindowLevelTriggered         (void);
    void onMenu3DTriggered                (void);
public:
    void  enableInteraction(void);
    void disableInteraction(void);
    void bounds(float& xmin, float& xmax, float& ymin, float& ymax, float& zmin, float& zmax);
    void cameraUp(double *coordinates);
    void cameraPosition(double *coordinates);
    void cameraFocalPoint(double *coordinates);
    void setCameraPosition(double x, double y, double z);
    void setCameraClippingRange(double nearRange, double farRange);

    QString cameraProjectionMode(void);
    double cameraViewAngle(void);
    double cameraZoom(void);

protected slots:
    void widgetDestroyed(void);

private:
    v3dViewPrivate *d;

    friend class v3dViewObserver;
};

dtkAbstractView *createV3dView(void);

#endif
