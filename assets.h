#ifndef ASSETS_H
#define ASSETS_H

#include <QPixmap>
#include <QString>
#include <QMap>

class Assets
{
public:
    static Assets& instance();
    void loadPixmap(const QString& name, const QString& path);
    const QPixmap& getPixmap(const QString& name) const;
    void loadAll();


private:
    Assets() = default;

    QMap<QString, QPixmap> pixmaps;
};

#endif // ASSETS_H
