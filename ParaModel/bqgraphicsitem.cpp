#include "bqgraphicsitem.h"
#include <QVector>
#include <QDebug>
#include <QMenu>
#include <QSpinBox>
#include <QWidgetAction>
#include <QGraphicsObject>
#include <QtMath>
#include <QPushButton>

BGraphicsItem::BGraphicsItem(QPointF center, QPointF edge, ItemType type)
	: m_center(center), m_edge(edge), m_type(type)
{
	m_pen_noSelected.setColor(QColor(0, 160, 230));
	m_pen_noSelected.setWidth(2);
	m_pen_isSelected.setColor(QColor(255, 0, 255));
	m_pen_isSelected.setWidth(2);

	this->setPen(m_pen_noSelected);
	this->setFlags(QGraphicsItem::ItemIsSelectable |
		QGraphicsItem::ItemIsMovable |
		QGraphicsItem::ItemIsFocusable);
}

void BGraphicsItem::focusInEvent(QFocusEvent* event)
{
	Q_UNUSED(event);
	this->setPen(m_pen_isSelected);
}

void BGraphicsItem::focusOutEvent(QFocusEvent* event)
{
	Q_UNUSED(event);
	if (nUnitType == 0)
	{
		QPen pen = QPen(Qt::yellow);
		pen.setStyle(Qt::DashLine);
		this->setPen(pen);
	}
	else
	{
		this->setPen(m_pen_noSelected);
	}
	setSelected(false);
}


//------------------------------------------------------------------------------
void BGraphicsItem::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
	Q_UNUSED(event)
		setCursor(QCursor(Qt::ClosedHandCursor));
	nOriPos[0] = event->scenePos().x();
	nOriPos[1] = event->scenePos().y();
	this->scene()->clearSelection();
	//选中跟随的元素 柱子、门、窗只能选中自己
	if (this->nUnitType == 1 || this->nUnitType > 4)
	{
		setSelected(true);
		return;
	} 
	QList<QGraphicsItem*> itemList = this->scene()->items();
	for (size_t i = 0; i < itemList.size(); i++)
	{ 
		if (itemList[i]->type() == BGraphicsItem::type())
		{
			BGraphicsItem* proxyWidget = qgraphicsitem_cast<BGraphicsItem*>(itemList[i]);
			if (proxyWidget->graphAngle == 0 || proxyWidget->graphAngle == 90)
			{
				int x1 = proxyWidget->getCenter().x();
				int x2 = this->getCenter().x();

			}
			if (proxyWidget->nUnitIdx == this->nUnitIdx)
			{
				itemList[i]->setSelected(true);
				continue;
			}
			if (this->graphAngle ==0 && proxyWidget->getCenter().y()==this->getCenter().y())
			{
				itemList[i]->setSelected(true);
				continue;
			}
			if (this->graphAngle == 90 && proxyWidget->getCenter().x() == this->getCenter().x())
			{
				itemList[i]->setSelected(true);
				continue;
			}
		}
	}
	setSelected(true);
}
void BGraphicsItem::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
	if (operMourse == Qt::LeftButton)
	{
		if (nUnitType == 1)
			return;
		QPointF oMove;
		oMove.setX(event->scenePos().x() - nOriPos[0]);
		oMove.setY(event->scenePos().y() - nOriPos[1]);
		emit SceneItemMove(nUnitType, nUnitIdx, oMove);
	}
}

void BGraphicsItem::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
	if (event->buttons() == Qt::LeftButton)
	{
		operMourse = Qt::LeftButton;
		if (nUnitType == 1)
			return;
		// 发送相对位移
		QPointF oMove;
		oMove.setX(event->scenePos().x() - nOriPos[0]);
		oMove.setY(event->scenePos().y() - nOriPos[1]);
		/*nOriPos[0] = event->scenePos().x();
		nOriPos[1] = event->scenePos().y();*/
		//emit SceneItemMove(nUnitType, nUnitIdx, oMove);
		// 发送当前绝对位置
// 		nMoveXY[0] = event->scenePos().x() - nOriPos[0]; 
// 		nMoveXY[1] = event->scenePos().y() - nOriPos[1]; 
// 		emit SceneItemMove(nUnitType, nUnitIdx, event->scenePos());
		QAbstractGraphicsShapeItem::mouseMoveEvent(event);
	}
}


BEllipse::BEllipse(qreal x, qreal y, qreal width, qreal height, ItemType type)
	: BGraphicsItem(QPointF(x, y), QPointF(x + width / 2, y + height / 2), type)
{
	/*   BPointItem *point = new BPointItem(this, m_edge, BPointItem::Edge);
	   point->setParentItem(this);
	   m_pointList.append(point);
	   m_pointList.append(new BPointItem(this, m_center, BPointItem::Center));
	   m_pointList.setRandColor();*/
}

QRectF BEllipse::boundingRect() const
{
	return QRectF(m_center.x() * 2 - m_edge.x(), m_center.y() * 2 - m_edge.y(), m_edge.x(), m_edge.y());
}

void BEllipse::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	Q_UNUSED(option);
	Q_UNUSED(widget);
	painter->setPen(this->pen());
	painter->setBrush(this->brush());
	//控制形状吸附网格
	m_center.setX(int(m_center.x() + 10) / 20 * 20);
	m_center.setY(int(m_center.y() + 10) / 20 * 20);
	m_edge.setX(int(m_edge.x() + 10) / 20 * 20);
	m_edge.setY(int(m_edge.y() + 10) / 20 * 20);
	//椭圆外矩形左上角点，宽，高  (x1+x3)/2=x2 - - - x1=2*x2-x3   宽（x3-x2）*2
	QRectF ret(m_center.x() * 2 - m_edge.x(), m_center.y() * 2 - m_edge.y(), (m_edge.x() - m_center.x()) * 2, (m_edge.y() - m_center.y()) * 2);
	painter->drawEllipse(ret);
}

void BEllipse::contextMenuEvent(QGraphicsSceneContextMenuEvent* event)
{
	if (!this->isSelected())
		return;

	QMenu* menu = new QMenu();
	menu->setStyleSheet("QMenu { background-color:rgb(89,87,87); border: 5px solid rgb(235,110,36); }");

	QSpinBox* width_spinBox = new QSpinBox(menu);
	width_spinBox->setStyleSheet("QSpinBox{ width:120px; height:30px; font-size:16px; font-weight:bold; }");
	width_spinBox->setRange(0, 1000);
	width_spinBox->setPrefix("w: ");
	width_spinBox->setSuffix(" mm");
	width_spinBox->setSingleStep(1);
	width_spinBox->setValue(2 * abs(m_edge.x()));
	connect(width_spinBox, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), [=](int v) {
		/*if (m_edge.x() < 0) {
			m_edge.setX((-1) * v/2);
		} else {
			m_edge.setX(v/2);
		}
		m_pointList.at(0)->setPoint(m_edge);*/
		this->hide();
		this->update();
		this->show();
		});

	QSpinBox* height__spinBox = new QSpinBox(menu);
	height__spinBox->setStyleSheet("QSpinBox{ width:120px; height:30px; font-size:16px; font-weight:bold; }");
	height__spinBox->setRange(0, 1000);
	height__spinBox->setPrefix("h: ");
	height__spinBox->setSuffix(" mm");
	height__spinBox->setSingleStep(1);
	height__spinBox->setValue(2 * abs(m_edge.y()));
	connect(height__spinBox, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), [=](int v) {
		/* if (m_edge.y() < 0) {
			 m_edge.setY((-1) * v/2);
		 } else {
			 m_edge.setY(v/2);
		 }
		 m_pointList.at(0)->setPoint(m_edge);*/
		this->hide();
		this->update();
		this->show();
		});

	QWidgetAction* width_widgetAction = new QWidgetAction(menu);
	width_widgetAction->setDefaultWidget(width_spinBox);
	menu->addAction(width_widgetAction);

	QWidgetAction* height_widgetAction = new QWidgetAction(menu);
	height_widgetAction->setDefaultWidget(height__spinBox);
	menu->addAction(height_widgetAction);

	menu->exec(QCursor::pos());
	delete menu;

	QGraphicsItem::contextMenuEvent(event);
}

//------------------------------------------------------------------------------

BCircle::BCircle(qreal x, qreal y, qreal radius, ItemType type)
	: BEllipse(x, y, radius* sqrt(2), radius* sqrt(2), type)
{
	updateRadius();
}

void BCircle::updateRadius()
{
	m_radius = sqrt(pow(m_center.x() - m_edge.x(), 2) + pow(m_center.y() - m_edge.y(), 2));
}

QRectF BCircle::boundingRect() const
{
	return QRectF(m_center.x() - m_radius, m_center.y() - m_radius, m_radius * 2, m_radius * 2);
}

void BCircle::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	Q_UNUSED(option);
	Q_UNUSED(widget);
	painter->setPen(this->pen());
	painter->setBrush(this->brush());
	//控制形状吸附网格
	m_center.setX(int(m_center.x() + 10) / 20 * 20);
	m_center.setY(int(m_center.y() + 10) / 20 * 20);
	m_radius = (int(m_radius + 10) / 20 * 20);
	QRectF ret(m_center.x() - m_radius, m_center.y() - m_radius, m_radius * 2, m_radius * 2);
	painter->drawEllipse(ret);
}

void BCircle::contextMenuEvent(QGraphicsSceneContextMenuEvent* event)
{
	if (!this->isSelected())
		return;

	QMenu* menu = new QMenu();
	menu->setStyleSheet("QMenu { background-color:rgb(89,87,87); border: 5px solid rgb(235,110,36); }");

	QSpinBox* radius_spinBox = new QSpinBox(menu);
	radius_spinBox->setStyleSheet("QSpinBox{ width:120px; height:30px; font-size:16px; font-weight:bold; }");
	radius_spinBox->setRange(0, 1000);
	radius_spinBox->setPrefix("r: ");
	radius_spinBox->setSuffix(" mm");
	radius_spinBox->setSingleStep(1);
	radius_spinBox->setValue(m_radius);
	connect(radius_spinBox, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), [=](int v) {
		m_radius = v;

		if (m_edge.x() < 0) {
			m_edge.setX(m_center.x() - m_radius * sqrt(2) / 2);
		}
		else {
			m_edge.setX(m_center.x() + m_radius * sqrt(2) / 2);
		}

		if (m_edge.y() < 0) {
			m_edge.setY(m_center.y() - m_radius * sqrt(2) / 2);
		}
		else {
			m_edge.setY(m_center.y() + m_radius * sqrt(2) / 2);
		}

		//m_pointList.at(0)->setPoint(m_edge);
		this->hide();
		this->update();
		this->show();
		});

	QWidgetAction* radius_widgetAction = new QWidgetAction(menu);
	radius_widgetAction->setDefaultWidget(radius_spinBox);
	menu->addAction(radius_widgetAction);

	menu->exec(QCursor::pos());
	delete menu;

	QGraphicsItem::contextMenuEvent(event);
}


//------------------------------------------------------------------------------

BRectangle::BRectangle(qreal x, qreal y, qreal width, qreal height, ItemType type)
	: BGraphicsItem(QPointF(x, y), QPointF(x + width / 2, y + height / 2), type)
{
	m_leftup.setX(int(m_center.x() - (m_edge.x()- m_center.x())  ));
	m_leftup.setY(int(m_center.y() - (m_edge.y() - m_center.y()) ));

}

QRectF BRectangle::boundingRect() const
{
	return QRectF(m_leftup.x(), m_leftup.y(), m_edge.x() - m_leftup.x(), m_edge.y() - m_leftup.y());
}

void BRectangle::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	Q_UNUSED(option);
	Q_UNUSED(widget);
	painter->setPen(this->pen());
	painter->setBrush(this->brush());
	 
	QRectF ret(m_leftup.x(), m_leftup.y(), m_edge.x() - m_leftup.x(), m_edge.y()- m_leftup.y());
	painter->drawRect(ret);
}

void BRectangle::contextMenuEvent(QGraphicsSceneContextMenuEvent* event)
{
	if (!this->isSelected())
		return;
	QMenu* menu = new QMenu();
	menu->setStyleSheet("QMenu { background-color:rgb(89,87,87); border: 5px solid rgb(235,110,36); }");

	QSpinBox* width_spinBox = new QSpinBox(menu);
	width_spinBox->setStyleSheet("QSpinBox{ width:120px; height:30px; font-size:16px; font-weight:bold; }");
	width_spinBox->setRange(0, 10000);
	width_spinBox->setPrefix("X: ");
	width_spinBox->setDisabled(true);
	width_spinBox->setSuffix(" mm");
	width_spinBox->setSingleStep(1);
	width_spinBox->setValue(2 * abs(m_center.x()));
	connect(width_spinBox, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), [=](int v) {
		//发送信号 确认修改后的值
		});

	QSpinBox* height_spinBox = new QSpinBox(menu);
	height_spinBox->setStyleSheet("QSpinBox{ width:120px; height:30px; font-size:16px; font-weight:bold; }");
	height_spinBox->setRange(0, 10000);
	height_spinBox->setPrefix("Y: ");
	height_spinBox->setDisabled(true);
	height_spinBox->setSuffix(" mm");
	height_spinBox->setSingleStep(1);
	height_spinBox->setValue(2 * abs(m_edge.y()));
	connect(height_spinBox, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), [=](int v) {
		//发送信号 确认修改后的值
		});




	QWidgetAction* width_widgetAction = new QWidgetAction(menu);
	width_widgetAction->setDefaultWidget(width_spinBox);
	menu->addAction(width_widgetAction);

	QWidgetAction* height_widgetAction = new QWidgetAction(menu);
	height_widgetAction->setDefaultWidget(height_spinBox);
	menu->addAction(height_widgetAction);


	//QPushButton* pApplyBtn = new QPushButton(QString::fromStdString(u8"确认"));
	//pApplyBtn->setFixedWidth(100);

	//connect(pApplyBtn, &QPushButton::clicked, this, [=]() {
	//	//发送信号 确认修改后的值 
	//	emit SceneMenuClick(nUnitType, nUnitIdx, 1);
	//	});


	//QWidgetAction* applyBtn_widgetAction = new QWidgetAction(menu);
	//applyBtn_widgetAction->setDefaultWidget(pApplyBtn);
	//menu->addAction(applyBtn_widgetAction);

	QAction* act = new QAction(this);

	if (nUnitType >= 4)
	{ 
		act->setObjectName((u8"增加构件"));
		act->setIcon(QIcon(":/qss/res/qss/White/506463.png"));
		act->setText((u8"增加构件"));
		act->setCheckable(true);
		connect(act, &QAction::triggered, this, [=]() {
			//发送信号 提示点击了更换
			emit SceneMenuAddClick(nUnitType, nUnitIdx);
			});
		menu->addAction(act);

		act = new QAction(this);
		act->setObjectName((u8"删除构件"));
		act->setIcon(QIcon(":/qss/res/qss/White/506463.png"));
		act->setText((u8"删除构件"));
		act->setCheckable(true);
		connect(act, &QAction::triggered, this, [=]() {
			//发送信号 提示点击了更换
			emit SceneMenuDeleteClick(nUnitType, nUnitIdx);
			});
		menu->addAction(act);

		act = new QAction(this);
		act->setObjectName((u8"更换构件"));
		act->setIcon(QIcon(":/qss/res/qss/White/506463.png"));
		act->setText((u8"更换构件"));
		act->setCheckable(true);
		connect(act, &QAction::triggered, this, [=]() {
			//发送信号 提示点击了更换
			emit SceneMenuClick(nUnitType, nUnitIdx);
			});
		menu->addAction(act);
	}
	else if (nUnitType == 0)
	{
		act->setObjectName((u8"增加构件"));
		act->setIcon(QIcon(":/qss/res/qss/White/506463.png"));
		act->setText((u8"增加构件"));
		act->setCheckable(true);
		connect(act, &QAction::triggered, this, [=]() {
			//发送信号 提示点击了更换
			emit SceneMenuAddClick(nUnitType, nUnitIdx);
			});
		menu->addAction(act);
	}
	else if (nUnitType == 1)
	{

		act->setObjectName((u8"更换构件"));
		act->setIcon(QIcon(":/qss/res/qss/White/506463.png"));
		act->setText((u8"更换构件"));
		act->setCheckable(true);
		connect(act, &QAction::triggered, this, [=]() {
			//发送信号 提示点击了更换
			emit SceneMenuClick(nUnitType, nUnitIdx);
			});
		menu->addAction(act);
	} 
	menu->exec(QCursor::pos());
	delete menu;

	QGraphicsItem::contextMenuEvent(event);
}

//------------------------------------------------------------------------------

BSquare::BSquare(qreal x, qreal y, qreal width, ItemType type)
	: BRectangle(x, y, width, width, type) {}

void BSquare::contextMenuEvent(QGraphicsSceneContextMenuEvent* event)
{
	if (!this->isSelected())
		return;

	QMenu* menu = new QMenu();
	menu->setStyleSheet("QMenu { background-color:rgb(89,87,87); border: 5px solid rgb(235,110,36); }");

	QSpinBox* distance_spinBox = new QSpinBox(menu);
	distance_spinBox->setStyleSheet("QSpinBox{ width:120px; height:30px; font-size:16px; font-weight:bold; }");
	distance_spinBox->setRange(0, 1000);
	distance_spinBox->setPrefix("d: ");
	distance_spinBox->setSuffix(" mm");
	distance_spinBox->setSingleStep(1);
	distance_spinBox->setValue(2 * abs(m_edge.x()));
	connect(distance_spinBox, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), [=](int v) {

		this->hide();
		this->update();
		this->show();
		});

	QWidgetAction* distance_widgetAction = new QWidgetAction(menu);
	distance_widgetAction->setDefaultWidget(distance_spinBox);
	menu->addAction(distance_widgetAction);

	menu->exec(QCursor::pos());
	delete menu;

	QGraphicsItem::contextMenuEvent(event);
}

//------------------------------------------------------------------------------

BPolygon::BPolygon(ItemType type)
	: BGraphicsItem(QPointF(0, 0), QPointF(0, 0), type)
{
	is_create_finished = false;
}

void BPolygon::pushPoint(QPointF p, QList<QPointF> list, bool isCenter)
{
	if (!is_create_finished) {
		m_center = getCentroid(list);
		getMaxLength();

		if (isCenter) {
			m_pointList.append(new BPointItem(this, m_center, BPointItem::Center));
			m_pointList.setRandColor();
			is_create_finished = true;
		}
		else {
			BPointItem* point = new BPointItem(this, p, BPointItem::Edge);
			point->setParentItem(this);
			m_pointList.append(point);
		}
		this->hide();
		this->update();
		this->show();
	}
}

QPointF BPolygon::getCentroid(QList<QPointF> list)
{
	qreal x = 0;
	qreal y = 0;
	for (auto& temp : list)
	{
		x += temp.x();
		y += temp.y();
	}
	x = x / list.size();
	y = y / list.size();
	return QPointF(x, y);
}

void BPolygon::getMaxLength()
{
	QVector<qreal> vec;
	for (auto& temp : m_pointList)
	{
		qreal dis = sqrt(pow(m_center.x() - temp->x(), 2) + pow(m_center.y() - temp->y(), 2));
		vec.append(dis);
	}

	qreal ret = 0;
	for (auto& temp : vec)
	{
		if (temp > ret) {
			ret = temp;
		}
	}
	m_radius = ret;
}

void BPolygon::updatePolygon(QPointF origin, QPointF end)
{
	QList<QPointF> list;

	for (auto& temp : m_pointList) {
		if (temp->getPoint() == origin) {
			temp->setPoint(end);
		}
		list.append(temp->getPoint());
	}

	m_center = getCentroid(list);
	m_pointList.at(m_pointList.size() - 1)->setPoint(m_center);

	getMaxLength();
}

QRectF BPolygon::boundingRect() const
{
	return QRectF(m_center.x() - m_radius, m_center.y() - m_radius, m_radius * 2, m_radius * 2);
}

void BPolygon::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	Q_UNUSED(option);
	Q_UNUSED(widget);
	painter->setPen(this->pen());
	painter->setBrush(this->brush());

	if (is_create_finished) {
		for (int i = 1; i < m_pointList.size() - 1; i++)
		{
			painter->drawLine(m_pointList.at(i - 1)->getPoint(), m_pointList.at(i)->getPoint());
		}

		painter->drawLine(m_pointList.at(m_pointList.size() - 2)->getPoint(), m_pointList.at(0)->getPoint());
	}
	else {
		for (int i = 1; i < m_pointList.size(); i++)
		{
			painter->drawLine(m_pointList.at(i - 1)->getPoint(), m_pointList.at(i)->getPoint());
		}
	}
}

//------------------------------------------------------------------------------
 
BLine::BLine(ItemType type)
	: BGraphicsItem(QPointF(0, 0), QPointF(0, 0), type)
{
	 
}
   
QRectF BLine::boundingRect() const
{
	return QRectF(m_center.x() - m_radius, m_center.y() - m_radius, m_radius * 2, m_radius * 2);
}

void BLine::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	Q_UNUSED(option);
	Q_UNUSED(widget);
	painter->setPen(this->pen());
	painter->setBrush(this->brush());
	//给出的关键点绘制线
	static QList<QPointF> points = QList<QPointF>() << QPointF(10, 40) << QPointF(100, 100) << QPointF(200, 100)
		<< QPointF(300, 100) << QPointF(330, 80) << QPointF(350, 70);

	QPainterPath path(points[0]);
	for (int i = 1; i < points.size(); ++i)
	{
		path.lineTo(points[i]);
	}
	painter->drawPath(path);
}

//------------------------------------------------------------------------------


BPoint::BPoint(ItemType type)
	: BGraphicsItem(QPointF(0, 0), QPointF(0, 0), type)
{

}

QRectF BPoint::boundingRect() const
{
	return QRectF(m_center.x() - m_radius, m_center.y() - m_radius, m_radius * 2, m_radius * 2);
}

void BPoint::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	Q_UNUSED(option);
	Q_UNUSED(widget);
	painter->setPen(this->pen());
	painter->setBrush(this->brush());
	//给出的关键点绘制线
	static QList<QPointF> points = QList<QPointF>() << QPointF(10, 40) << QPointF(100, 100) << QPointF(200, 100)
		<< QPointF(300, 100) << QPointF(330, 80) << QPointF(350, 70);

	int ellipse_size = 4;
	for (int i = 0; i < points.size(); ++i) {
		painter->drawEllipse(points[i], ellipse_size, ellipse_size);
		QString text = QString::number(points[i].y());
		QFontMetrics fm = painter->fontMetrics();
		int height_font = fm.height();
		int width_text = fm.width(text);
		QPointF textPos(points[i].x() - (width_text / 2), points[i].y() - (ellipse_size / 2) - (height_font / 2));
		painter->drawText(textPos, text);
	}
}

//------------------------------------------------------------------------------