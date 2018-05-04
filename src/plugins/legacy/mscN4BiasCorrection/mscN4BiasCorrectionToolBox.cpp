/*=========================================================================

 medInria

 Copyright (c) INRIA 2013. All rights reserved.
 See LICENSE.txt for details.
 
  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#include "mscN4BiasCorrection.h"
#include "mscN4BiasCorrectionToolBox.h"

#include <QtGui>

#include <dtkCoreSupport/dtkAbstractProcessFactory.h>
#include <dtkCoreSupport/dtkAbstractProcess.h>
#include <dtkCoreSupport/dtkAbstractViewFactory.h>
#include <dtkCoreSupport/dtkSmartPointer.h>

#include <medAbstractView.h>
#include <medRunnableProcess.h>

#include <medAbstractImageData.h>
//#include <medFilteringSelectorToolBox.h>
//#include <medSegmentationSelectorToolBox.h>
#include <medReformatSelectorToolBox.h>
#include <medToolBoxFactory.h>
#include <medPluginManager.h>

#include <sstream>

class mscN4BiasCorrectionToolBoxPrivate
{
public:
    
    mscN4BiasCorrection* process;
    QDoubleSpinBox * splineDistanceSpinBox, *widthSpinBox,
    *convThresholdSpinBox, *wienerNoiseSpinBox;
    QSpinBox *bsplineOrderSpinBox, *shrinkFactorSpinBox, *nbHistogramBinsSpinBox;
    QLineEdit *splineGridResolutionText, *nbOfIterationsText;
    QCheckBox * saveBiasCheckBox;
};

mscN4BiasCorrectionToolBox::mscN4BiasCorrectionToolBox(QWidget *parent) : medReformatAbstractToolBox(parent), d(new mscN4BiasCorrectionToolBoxPrivate)
{
    QWidget *widget = new QWidget(this);
    
    QPushButton *runButton = new QPushButton(tr("Run"), this);
    runButton->setObjectName("runButton");

    QLabel* splineDistanceLabel = new QLabel (tr("Spline Distance"));
    d->splineDistanceSpinBox = new QDoubleSpinBox;
    d->splineDistanceSpinBox->setMaximum(500);
    d->splineDistanceSpinBox->setMinimum(0);
    d->splineDistanceSpinBox->setValue(200);
    QHBoxLayout * splineDistanceLayout = new QHBoxLayout();
    splineDistanceLayout->addWidget(splineDistanceLabel);
    splineDistanceLayout->addWidget(d->splineDistanceSpinBox);

    QLabel* widthLabel = new QLabel (tr("Bias Field Full Width at Half Maximum"));
    widthLabel->setToolTip(
        "Parameter characterizing the width of the Gaussian deconvolution.\n Zero implies use of the default value = 0.15.");
    d->widthSpinBox = new QDoubleSpinBox;
    d->widthSpinBox->setMaximum(10);
    d->widthSpinBox->setMinimum(0);
    d->widthSpinBox->setValue(2);
    QHBoxLayout * widthLayout = new QHBoxLayout();
    widthLayout->addWidget(widthLabel);
    widthLayout->addWidget(d->widthSpinBox);

    QLabel * nbOfIterationsLabel = new QLabel("Number of Iterations");
    nbOfIterationsLabel->setToolTip(
        "Maximum number of iterations at each resolution.\nThe number of resolutions is implicitely defined by this parameter\n(the size of this list is the number of resolutions)");
    d->nbOfIterationsText = new QLineEdit;
    d->nbOfIterationsText->setText("200,100,100,50");
    QHBoxLayout * nbOfIterationsLayout = new QHBoxLayout();
    nbOfIterationsLayout->addWidget(nbOfIterationsLabel);
    nbOfIterationsLayout->addWidget(d->nbOfIterationsText);

    QLabel* convThresholdLabel = new QLabel (tr("Convergence threshold"));
    convThresholdLabel->setToolTip(
        "Convergence is determined by the coefficient of variation of the\ndifference image between the current bias field estimate and the previous estimate.\n If this value is less than the specified threshold, the algorithm\nproceeds to the next fitting level or terminates if it is at the last level. ");
    d->convThresholdSpinBox = new QDoubleSpinBox;
    d->convThresholdSpinBox->setMaximum(1);
    d->convThresholdSpinBox->setMinimum(0);
    d->convThresholdSpinBox->setDecimals(4);
    d->convThresholdSpinBox->setValue(0.001);
    QHBoxLayout * convThresholdLayout = new QHBoxLayout();
    convThresholdLayout->addWidget(convThresholdLabel);
    convThresholdLayout->addWidget(d->convThresholdSpinBox);

    QLabel* bsplineOrderLabel = new QLabel (tr("B-Spline Order"));
    bsplineOrderLabel->setToolTip(
        "Spline order defining the bias field estimate.\nCubic splines (order = 3) are typically used.");
    d->bsplineOrderSpinBox = new QSpinBox;
    d->bsplineOrderSpinBox->setMaximum(10);
    d->bsplineOrderSpinBox->setMinimum(0);
    d->bsplineOrderSpinBox->setValue(3);
    QHBoxLayout * bsplineOrderLayout = new QHBoxLayout();
    bsplineOrderLayout->addWidget(bsplineOrderLabel);
    bsplineOrderLayout->addWidget(d->bsplineOrderSpinBox);

    QLabel* shrinkFactorLabel = new QLabel (tr("Shrink Factor"));
    shrinkFactorLabel->setToolTip(
        "To lessen computation time, the input image can be resampled.\nThe shrink factor, specified as a single integer, describes\nthis resampling.  Shrink factors <= 4 are commonly used.");
    d->shrinkFactorSpinBox = new QSpinBox;
    d->shrinkFactorSpinBox->setMaximum(10);
    d->shrinkFactorSpinBox->setMinimum(0);
    d->shrinkFactorSpinBox->setValue(4);
    QHBoxLayout * shrinkFactorLayout = new QHBoxLayout();
    shrinkFactorLayout->addWidget(shrinkFactorLabel);
    shrinkFactorLayout->addWidget(d->shrinkFactorSpinBox);

    QLabel* nbHistogramBinsLabel = new QLabel (tr("Number of Histogram Bins"));
    nbHistogramBinsLabel->setToolTip(
        "Number of bins defining the log input intensity histogram.\nZero implies use of the default value = 200.");
    d->nbHistogramBinsSpinBox = new QSpinBox;
    d->nbHistogramBinsSpinBox->setMaximum(1000);
    d->nbHistogramBinsSpinBox->setMinimum(0);
    d->nbHistogramBinsSpinBox->setValue(200);
    QHBoxLayout * nbHistogramBinsLayout = new QHBoxLayout();
    nbHistogramBinsLayout->addWidget(nbHistogramBinsLabel);
    nbHistogramBinsLayout->addWidget(d->nbHistogramBinsSpinBox);

    QLabel* wienerNoiseLabel = new QLabel (tr("Wiener Filter Noise"));
    wienerNoiseLabel->setToolTip(
        "Noise estimate defining the Wiener filter.\nZero implies use of the default value = 0.01.");
    d->wienerNoiseSpinBox = new QDoubleSpinBox;
    d->wienerNoiseSpinBox->setMaximum(1);
    d->wienerNoiseSpinBox->setMinimum(0);
    d->wienerNoiseSpinBox->setValue(0.01);
    QHBoxLayout * wienerNoiseLayout = new QHBoxLayout();
    wienerNoiseLayout->addWidget(wienerNoiseLabel);
    wienerNoiseLayout->addWidget(d->wienerNoiseSpinBox);

    QLabel* saveBiasLabel = new QLabel (tr("Save bias field"));
    d->saveBiasCheckBox = new QCheckBox();
    d->saveBiasCheckBox->setChecked(false);
    QHBoxLayout * saveBiasLayout = new QHBoxLayout();
    saveBiasLayout->addWidget(saveBiasLabel);
    saveBiasLayout->addWidget(d->saveBiasCheckBox);
    
    QVBoxLayout * layout = new QVBoxLayout();
    layout->addLayout(splineDistanceLayout);
    layout->addLayout(widthLayout);
    layout->addLayout(nbOfIterationsLayout);
    layout->addLayout(convThresholdLayout);
    layout->addLayout(bsplineOrderLayout);
    layout->addLayout(shrinkFactorLayout);
    layout->addLayout(nbHistogramBinsLayout);
    layout->addLayout(wienerNoiseLayout);
    layout->addLayout(saveBiasLayout);
    layout->addWidget(runButton);
    widget->setLayout(layout);
    this->addWidget(widget);
    
    connect(runButton, SIGNAL(clicked()), this, SLOT(run()));

}

mscN4BiasCorrectionToolBox::~mscN4BiasCorrectionToolBox()
{
    delete d;
    
    d = NULL;
}

bool mscN4BiasCorrectionToolBox::registered()
{
    return medToolBoxFactory::instance()->registerToolBox<mscN4BiasCorrectionToolBox>();
}

dtkPlugin* mscN4BiasCorrectionToolBox::plugin()
{
    medPluginManager* pm = medPluginManager::instance();
    dtkPlugin* plugin = pm->plugin ( "N4 Bias Correction" );
    return plugin;
}

medAbstractData* mscN4BiasCorrectionToolBox::processOutput()
{
    qDebug()<<"### mscN4BiasCorrectionToolBox::processOutput";

    if(!d->process || !d->process->output())
    {
        run();
    }

    if (d->process)
    {
        return d->process->output();
    }
    else
    {
        return 0;
    }
}

void mscN4BiasCorrectionToolBox::run()
{
    qDebug()<<"### mscN4BiasCorrectionToolBox::run";

    if(this->parentToolBox())
    {
        qDebug()<<"### mscN4BiasCorrectionToolBox::run AA";

        if(this->parentToolBox()->fixedData())
        {
            qDebug()<<"### mscN4BiasCorrectionToolBox::run BB";

            this->setToolBoxOnWaitStatus();

            d->process = new mscN4BiasCorrection();
            d->process->setInput(this->parentToolBox()->fixedData());

            d->process->setParameter(d->splineDistanceSpinBox->value(), 0);
            d->process->setParameter(d->widthSpinBox->value(), 1);
            d->process->setParameter(d->convThresholdSpinBox->value(), 2);
            d->process->setParameter((double)d->bsplineOrderSpinBox->value(), 3);
            d->process->setParameter((double)d->shrinkFactorSpinBox->value(), 4);
            d->process->setParameter((double)d->nbHistogramBinsSpinBox->value(), 5);
            d->process->setParameter(d->wienerNoiseSpinBox->value(), 6);
            d->process->setParameter((double)d->saveBiasCheckBox->isChecked(), 7);

            std::vector<int> nbOfIterations = this->extractValue(d->nbOfIterationsText->text());
            for (unsigned int i = 0; i<nbOfIterations.size();i++)
                d->process->setParameter((double)nbOfIterations[i], 10+i);

            medRunnableProcess *runProcess = new medRunnableProcess;
            runProcess->setProcess (d->process);
            this->addConnectionsAndStartJob(runProcess);
        }
    }
}

std::vector<int> mscN4BiasCorrectionToolBox::extractValue(QString text)
{
    std::vector<int> out;
    std::stringstream stream(text.toStdString());
    
    int i;
    while ( stream >> i)
    {
        out.push_back(i);

        if(stream.peek()== ',')
            stream.ignore();
    }
    return out;
}
