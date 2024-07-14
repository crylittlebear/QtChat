#pragma once

#include <QStackedWidget>
#include "qpropertyanimation.h"

class RotateStackedWidget  : public QStackedWidget
{
	Q_OBJECT
	Q_PROPERTY(float rotateVal READ rotateVal WRITE setRotateVal)

public:
	/*!
	* @brief RotateStackedWidget constructor
	*/
	RotateStackedWidget(QWidget *parent);

	/*!
	* @brief RotateStackedWidget destructor
	*/
	~RotateStackedWidget();

	/*!
	* @brief stacked widget page jumped
	*/
	void rotate(int index);

	/*!
	* @brief set rotateVal
	*/
	void setRotateVal(float val) { rotateVal_ = val; }

	/*!
	* @brief get rotateVal
	*/
	float rotateVal() const { return rotateVal_; }

public slots:
	/*!
	* @brief jump to next page
	*/
	void nextPage();

protected:
	virtual void paintEvent(QPaintEvent* event) override;

private slots:
	/*!
	* @brief rotate value changed
	*/
	void valChanged(const QVariant& value);

	/*!
	* @brief animation done
	*/
	void animationDone();

private:
	float rotateVal_;
	bool isAnimating_;
	int nextIndex_;
};
