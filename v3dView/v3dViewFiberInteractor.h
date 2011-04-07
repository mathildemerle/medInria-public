#ifndef V3DVIEWFIBERINTERACTOR_H
#define V3DVIEWFIBERINTERACTOR_H

#include <medCore/medAbstractViewFiberInteractor.h>

#include "v3dViewPluginExport.h"

class v3dViewFiberInteractorPrivate;

class dtkAbstractData;
class dtkAbstractView;

class V3DVIEWPLUGIN_EXPORT v3dViewFiberInteractor: public medAbstractViewFiberInteractor
{
    Q_OBJECT

public:
    v3dViewFiberInteractor();
    virtual ~v3dViewFiberInteractor();

    virtual QString description(void) const;
    virtual QStringList handled(void) const;

    static bool registered(void);

    virtual void setData(dtkAbstractData *data);
    dtkAbstractData *data (void);

    virtual void setView(dtkAbstractView *view);
    dtkAbstractView *view (void);

    virtual void enable(void);
    virtual void disable(void);

    virtual void setROI(dtkAbstractData *data);

    virtual void setRoiBoolean (int roi, int meaning);
    virtual int  roiBoolean (int roi);

    virtual void setBundleVisibility(const QString &name, bool visibility);
    virtual bool bundleVisibility(const QString &name) const;

    virtual void setBundleVisibility(bool visibility);

public slots:
    virtual void onPropertySet (const QString& key, const QString& value);

    virtual void onVisibilityPropertySet (const QString& value);
    virtual void onBoxVisibilityPropertySet (const QString& value);
    virtual void onRenderingModePropertySet (const QString& value);
    virtual void onGPUModePropertySet (const QString& value);
    virtual void onColorModePropertySet (const QString& value);
    virtual void onBoxBooleanOperationPropertySet (const QString& value);
    virtual void onProjectionPropertySet (const QString& value);
    virtual void onRadiusSet (int value);

    virtual void onSelectionTagged (void);
    virtual void onSelectionReset (void);
    virtual void onSelectionValidated (const QString &name, const QColor &color);

    virtual void onBundlingBoxBooleanOperatorChanged(int value);

protected:
    virtual void computeBundleFAStatistics (const QString &name,
                                            double &mean,
                                            double &min,
                                            double &max,
                                            double &var);

    virtual void computeBundleADCStatistics (const QString &name,
                                             double &mean,
                                             double &min,
                                             double &max,
                                             double &var);

    virtual void computeBundleLengthStatistics (const QString &name,
                                                double &mean,
                                                double &min,
                                                double &max,
                                                double &var);
private:
    v3dViewFiberInteractorPrivate *d;

};

dtkAbstractViewInteractor *createV3dViewFiberInteractor(void);

#endif
