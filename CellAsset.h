#pragma once

#include "Asset.h"
#include "Storage.h"

class PartConfig;
class Part_BodyAsset;
class Part_NucleusAsset;

class CellAsset : public Asset {
  Storage m_material;
  Storage m_maxStorage;

  Array<std::shared_ptr<PartConfig>> m_partConfigs;

  // Rigidbody
  double m_mass;
  double m_radius;
  double m_inertia;
  double m_drawRadius;

  // timer
  double m_lifespanTime; // 寿命
  double m_bornTime;     // 生まれるまでの時間
  double m_yieldTime;    // 生むのにかかる時間

  double m_maxHitPoint;

  void updateMass();
  void updateRadius() { m_radius = sqrt(2 * m_inertia / m_mass); }
  void updateDrawRadius();
  void updateInertia();
  void updateMaxStorage();
  void updateMaterial();

  Texture m_cellAssetTexture; // 全てのパーツが描画されたTexture
  Texture m_cellStateTexture; // 静的な描画のパーツのみ描画されたTexture

  RectF getCellAssetDrawRegion() const;
  RectF getCellStateDrawRegion() const;

public:
  const Texture& getCellAssetTexture() const { return m_cellAssetTexture; }
  const Texture& getCellStateTexture() const { return m_cellStateTexture; }

  void preRender();

  bool m_isInViewer = false;

  bool isValid() const;

  Vec2 getCentroid();

  void setCentroidAsOrigin();

  void removePartConfig(const std::shared_ptr<PartConfig>& partConfig) {
    m_partConfigs.remove(partConfig);
  }

  // Get
  const Storage& getMaterial() const { return m_material; }
  const Storage& getMaxStorage() const { return m_maxStorage; }

  double getMass() const { return m_mass; }
  double getRadius() const { return m_radius; }
  double getDrawRadius() const { return m_drawRadius; }
  double getInertia() const { return m_inertia; }

  const Array<std::shared_ptr<PartConfig>>& getPartConfigs() const { return m_partConfigs; }

  double getLifespanTime() const { return m_lifespanTime; }
  double getBornTime() const { return m_bornTime; }
  double getYieldTime() const { return m_yieldTime; }
  double getMaxHitPoint() const { return m_maxHitPoint; }

  // Add
  std::shared_ptr<PartConfig>& addPartConfig();

  void updateProperties();

  std::shared_ptr<Part_BodyAsset>    getBodyAsset() const;
  std::shared_ptr<Part_NucleusAsset> getNucleusAsset() const;

  // draw
  void draw(double a = 0.5);

  // JSON
  void load(const JSON& json) override;
  void save(JSON& json) const override;

  // TypeName
  String getTypeName() override { return U"CellAsset"; }

  class Log {
  public:
    class Status {
    public:
      int m_numCell;
      int m_numEgg;

      Status(int numCell, int numEgg)
          : m_numCell(numCell)
          , m_numEgg(numEgg) { }
    };

    Array<Status> m_statuses;

    void update(const std::shared_ptr<CellAsset>& cellAsset);
  } m_log;
};
