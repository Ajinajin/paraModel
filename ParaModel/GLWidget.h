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
	// Shift+左键：进行选择  Alt：准备缩放
	void mousePressEvent(QGraphicsSceneMouseEvent *event);
	// Alt+拖拽：进行缩放  移动
	void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
	// 使item可使用qgraphicsitem_cast

    //键盘输入响应
    void handleKeyPressEvent(QKeyEvent* event);
    void handleKeyRleaseEvent(QKeyEvent* event);
    
private:
    QColor brushColor;
    const QColor colors[2] = { QColor{100, 100, 100, 100}, QColor(100, 100, 0, 200) };
};