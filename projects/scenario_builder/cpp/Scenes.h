#ifndef PFC_SCENARIO_BUILDER_SCENE_H
#define PFC_SCENARIO_BUILDER_SCENE_H

#include <QAbstractItemModel>
#include <QDate>
#include <QDebug>
#include <QElapsedTimer>
#include <QString>
#include <QVariant>

#include "MilitaryScenario_1.0.0.hxx"
#include "MsdlComplexTypes_1.0.0.hxx"

namespace pfc {

class NarativeSequence : public QAbstractItemModel {
  Q_OBJECT

  Q_PROPERTY(int32_t count READ rowCount() NOTIFY countChanged)

public:
  NarativeSequence(QObject* parent = nullptr);
  virtual ~NarativeSequence();
  NarativeSequence(xsd::cxx::tree::sequence<::msdl_1::InstallationType>&, QObject* parent = nullptr);
  NarativeSequence(const NarativeSequence&);
  NarativeSequence(NarativeSequence&&) = default;
  NarativeSequence& operator=(const NarativeSequence&);
  NarativeSequence& operator=(NarativeSequence&&) = default;

  enum SceneRoles {
    IdentityRole = Qt::UserRole + 1,
    SymbolRole,
    NameRole,
    OwnerRole,
    AffiliationRole,
    LatitudeRole,
    LongitudeRole,
    ElevationRole,
    DescriptionRole,
  };

  QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const override;
  QModelIndex parent(const QModelIndex& index) const override;
  int columnCount(const QModelIndex& parent = QModelIndex()) const override;
  int rowCount(const QModelIndex& parent = QModelIndex()) const override;
  QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

  Qt::ItemFlags flags(const QModelIndex& index) const override;
  bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;
  bool removeRows(int row, int count, const QModelIndex& parent = QModelIndex()) override;
  bool insertRows(int row, int count, const QModelIndex& parent = QModelIndex()) override;

public slots:
  void new_scene();
  void drop_scene(int);

signals:
  void countChanged();
  int32_t sceneAdded();
  int32_t sceneDroped();
  int32_t sceneModified();

protected:
  QHash<int, QByteArray> roleNames() const
  {
    QHash<int, QByteArray> roles;
    roles[Qt::DisplayRole] = "content";
    roles[Qt::DecorationRole] = "decoration";
    roles[Qt::EditRole] = "edit";
    roles[Qt::ToolTipRole] = "toolTip";
    roles[Qt::StatusTipRole] = "statusTip";
    roles[Qt::WhatsThisRole] = "whatsThis";
    roles[IdentityRole] = "id";
    roles[SymbolRole] = "symbol";
    roles[NameRole] = "name";
    roles[OwnerRole] = "owner";
    roles[AffiliationRole] = "affiliation";
    roles[LatitudeRole] = "lat";
    roles[LongitudeRole] = "lon";
    roles[ElevationRole] = "alt";
    roles[DescriptionRole] = "description";

    return roles;
  }

private:
  struct Implementation;
  std::unique_ptr<Implementation> _impl;

}; //class Actor
} //namespace pfc
#endif //PFC_SCENARIO_BUILDER_SCENE_H