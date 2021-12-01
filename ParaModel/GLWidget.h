//
#include <QGraphicsItem>
class MyItem : public QGraphicsItem
{
public:
    MyItem();
    QRectF boundingRect() const;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
        QWidget* widget);

    void setColor(const QColor& color) { brushColor = color; }
    std::vector<float> points;
protected:
	// Shift+���������ѡ��  Alt��׼������
	void mousePressEvent(QGraphicsSceneMouseEvent *event);
	// Alt+��ק����������  �ƶ�
	void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
	// ʹitem��ʹ��qgraphicsitem_cast

    //����������Ӧ
    void handleKeyPressEvent(QKeyEvent* event);
    void handleKeyRleaseEvent(QKeyEvent* event);
    
private:
    QColor brushColor;
    const QColor colors[2] = { QColor{100, 100, 100, 100}, QColor(100, 100, 0, 200) };
};