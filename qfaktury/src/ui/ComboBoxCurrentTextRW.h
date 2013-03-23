#ifndef COMBOBOXCURRENTTEXTRW_H
#define COMBOBOXCURRENTTEXTRW_H

#include <QComboBox>

/**
 * @brief
 *
 */
class ComboBoxCurrentTextRW : public QComboBox
{
    Q_OBJECT
    Q_PROPERTY(QString currentText READ currentString WRITE setCurrentString)
public:
    /**
     * @brief
     *
     * @param parent
     */
    explicit ComboBoxCurrentTextRW(QWidget *parent = 0);

    /**
     * @brief
     *
     * @return QString
     */
    QString currentString() const;
    /**
     * @brief
     *
     * @param str
     */
    void setCurrentString(const QString &str);
};

#endif
