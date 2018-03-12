#ifndef CCOMBOBOXPOPUPITEMDELEGATE_H
#define CCOMBOBOXPOPUPITEMDELEGATE_H
#include <QSize>
#include <QStyledItemDelegate>

class CComboBoxPopupItemDelegate : public QStyledItemDelegate
{
public:
     CComboBoxPopupItemDelegate(QObject* parent = 0)
         : QStyledItemDelegate(parent)
     {
     }

     QSize sizeHint(const QStyleOptionViewItem & option, const QModelIndex & index) const
     {
         return QSize(40, 40); //enter your values here
     }
};

#endif // CCOMBOBOXPOPUPITEMDELEGATE_H
