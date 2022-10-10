#include "GUIButton.h"
#include "GUIIcon.h"
#include "MainViewer.h"

void MainViewer::CommandPalette::init()
{
  setViewerSize(65 * 5 + 5, 65 * 2 + 10);
  setFirstPosInLocal(Scene::Width() - getViewerSize().x - 20, 20);
  setSecondPosInLocal(Scene::Width(), 20);

  const Color disableColor = Palette::Black;
  const Color enableColor = Palette::Red;

  addChildViewer<GUIButton>()
    ->setName(U"zoomIn")
    ->setViewerRectInLocal(5, 5, 60, 60)
    ->addChildViewer<GUIIcon>(0xf00e, 0.75, disableColor);

  addChildViewer<GUIButton>()
    ->setName(U"zoomOut")
    ->setViewerRectInLocal(70, 5, 60, 60)
    ->addChildViewer<GUIIcon>(0xf010, 0.75, disableColor);

  addChildViewer<GUIButton>()
    ->setName(U"fastMode")
    ->setViewerRectInLocal(135, 5, 60, 60)
    ->addChildViewer<GUIIcon>(0xf050, 0.75, disableColor);

  addChildViewer<GUIButton>()
    ->setName(U"showWave")
    ->setViewerRectInLocal(200, 5, 60, 60)
    ->addChildViewer<GUIIcon>(0xf5c4, 0.75, disableColor);

  addChildViewer<GUIButton>()
    ->setName(U"closeEye")
    ->setViewerRectInLocal(265, 5, 60, 60)
    ->addChildViewer<GUIIcon>(0xf070, 0.75, disableColor);

  addChildViewer<GUIButton>()
    ->setName(U"handNone")
    ->setViewerRectInLocal(5, 75, 60, 60)
    ->addChildViewer<GUIIcon>(0xf25a, 0.75, disableColor);

  addChildViewer<GUIButton>()
    ->setName(U"handPoison")
    ->setViewerRectInLocal(70, 75, 60, 60)
    ->addChildViewer<GUIIcon>(0xf714, 0.75, disableColor);

  addChildViewer<GUIButton>()
    ->setName(U"handAddElement")
    ->setViewerRectInLocal(135, 75, 60, 60)
    ->addChildViewer<GUIIcon>(0xf613, 0.75, disableColor);

  addChildViewer<GUIButton>()
    ->setName(U"handTrash")
    ->setViewerRectInLocal(200, 75, 60, 60)
    ->addChildViewer<GUIIcon>(0xf1f8, 0.75, disableColor);
}

void MainViewer::CommandPalette::update()
{
  HiddenViewer::update();

  RectF(getViewerSize()).rounded(5).draw(Palette::White).drawFrame(2.0, 0.0, Palette::Black);

  moveDrawPos(5, 5);

  const Color disableColor = Palette::Black;
  const Color enableColor = Palette::Red;

  const auto rect = RectF(60, 60).rounded(5);
  const auto fv = getParentViewer()->getChildViewer<FieldViewer>();

  const auto zoomIn = getChildViewer<GUIButton>(U"zoomIn");
  const auto zoomOut = getChildViewer<GUIButton>(U"zoomOut");
  const auto fastMode = getChildViewer<GUIButton>(U"fastMode");
  const auto showWave = getChildViewer<GUIButton>(U"showWave");
  const auto closeEye = getChildViewer<GUIButton>(U"closeEye");
  const auto handNone = getChildViewer<GUIButton>(U"handNone");
  const auto handPoison = getChildViewer<GUIButton>(U"handPoison");
  const auto handAddElement = getChildViewer<GUIButton>(U"handAddElement");
  const auto handTrash = getChildViewer<GUIButton>(U"handTrash");

  const auto zoomInIcon = zoomIn->getChildViewer<GUIIcon>();
  const auto zoomOutIcon = zoomOut->getChildViewer<GUIIcon>();
  const auto fastModeIcon = fastMode->getChildViewer<GUIIcon>();
  const auto showWaveIcon = showWave->getChildViewer<GUIIcon>();
  const auto closeEyeIcon = closeEye->getChildViewer<GUIIcon>();
  const auto handNoneIcon = handNone->getChildViewer<GUIIcon>();
  const auto handPoisonIcon = handPoison->getChildViewer<GUIIcon>();
  const auto handAddElementIcon = handAddElement->getChildViewer<GUIIcon>();
  const auto handTrashIcon = handTrash->getChildViewer<GUIIcon>();

  for (const auto button : getChildViewers<GUIButton>())
    button->getChildViewer<GUIIcon>()->setColor(disableColor);

  {
    if (zoomIn->isGrabbed() || KeyK.pressed())
    {
      zoomInIcon->setColor(enableColor);
      fv->getCamera().zoomIn();
    }
    if (zoomOut->isGrabbed() || KeyL.pressed())
    {
      zoomOutIcon->setColor(enableColor);
      fv->getCamera().zoomOut();
    }
    if (fastMode->isSelected())
    {
      fv->m_isHighSpeed = !fv->m_isHighSpeed;
      if (fv->m_isHighSpeed) { fastModeIcon->setColor(enableColor); }
    }
    if (showWave->isSelected())
    {
      fv->m_drawWaveEnabled = !fv->m_drawWaveEnabled;
      if (fv->m_drawWaveEnabled) { showWaveIcon->setColor(enableColor); }
    }
    if (closeEye->isSelected()) getParentViewer<MainViewer>()->setHiddenMode();
  }

  // Mode
  {
    if (handNone->isSelected()) fv->m_handAction = FieldViewer::HandAction::None;
    if (handPoison->isSelected()) fv->m_handAction = FieldViewer::HandAction::Poison;
    if (handAddElement->isSelected()) fv->m_handAction = FieldViewer::HandAction::AddElement;
    if (handTrash->isSelected()) fv->m_handAction = FieldViewer::HandAction::Trash;

    switch (fv->m_handAction)
    {
    case FieldViewer::HandAction::None: handNoneIcon->setColor(enableColor); break;
    case FieldViewer::HandAction::AddElement: handAddElementIcon->setColor(enableColor); break;
    case FieldViewer::HandAction::Poison: handPoisonIcon->setColor(enableColor); break;
    case FieldViewer::HandAction::Trash: handTrashIcon->setColor(enableColor); break;
    }
  }
}
