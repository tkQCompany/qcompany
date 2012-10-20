#include "ComboBoxCurrentTextRW.h"

/**
 * @brief
 *
 * @param parent
 */
ComboBoxCurrentTextRW::ComboBoxCurrentTextRW(QWidget *parent) :
    QComboBox(parent)
{
}


QString ComboBoxCurrentTextRW::currentString() const
{
    return currentText();
}



/**
 * @brief The method doesn't exist in the base class and is used with QDataWidgetMapper
 *
 * By default, QDataWidgetMapper uses currentIndex property. But if we need using "currentText" property instead,
then there is a problem with QDataWidgetMapper because there is no setter for the property (i.e. setCurrentText()).
Sets the current index of the combobox to the item of the same textual content as the parameter.
 *
 * @param str source string - must be the same as one of the combobox items (if not, then the method does nothing).
 */
void ComboBoxCurrentTextRW::setCurrentString(const QString &str)
{
    const int ind = findText(str);
    if(ind != -1)
    {
        setCurrentIndex(ind);
    }
}
