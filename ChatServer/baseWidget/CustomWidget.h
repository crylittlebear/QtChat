#pragma once

#include <QWidget>

/*!
*  @brief ������ʹ�õĴ�����Ļ���,���������϶�����������
*/
class CustomWidget : public QWidget
{
	Q_OBJECT

public:
	/*!
	* @brief ���캯��
	*/
	CustomWidget(QWidget* parent = nullptr);

	/*!
	* @brief ��������
	*/
	~CustomWidget();

protected:
	/*!
	* @brief ��д��갴���¼�
	*/
	virtual void mousePressEvent(QMouseEvent* event) override;

	/*!
	* @brief ��д����϶��¼�
	*/
	virtual void mouseMoveEvent(QMouseEvent* event) override;

	/*!
	* @brief ��д����ͷ��¼�
	*/
	virtual void mouseReleaseEvent(QMouseEvent* event) override;

private:
	/*!
	* @brief �ж�����Ƿ����϶�������
	*/
	bool isInDragingArea(const QPoint& point) const;

	/*!
	* @brief �ж�����Ƿ�������������
	*/
	bool isInScalingArea(const QPoint& point) const;

	/*!
	* @brief �ж��Ƿ��ڴ����Ҳ���������
	*/
	bool isInRightScalingArea(const QPoint& point) const;

	/*!
	* @brief �ж��Ƿ��ڴ����²���������
	*/
	bool isInBottomScalingArea(const QPoint& point) const;

	/*!
	* @brief �ж��Ƿ��ڴ����Ҳ���������
	*/
	bool isInBottomRightScalingArea(const QPoint& point) const;

private:
	int dragHeight_;
	bool isDraging_ = false;	/// �Ƿ������϶���
	bool isScaling_ = false;	/// �Ƿ�����������
	QPoint dragStartPoint_;		/// �϶���ʼ���λ�ô���������
};
