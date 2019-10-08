#pragma once

class MoleculeEditor
	: public ModelEditor
{
	TextEditState		m_textEditState_mass;

public:
	// Reload
	void reloadProperties_this()
	{
		ModelEditor::reloadProperties_this();

		m_textEditState_mass.text = ToString(getModel<MoleculeAsset>()->m_mass);
	}
	void reloadProperties() override { reloadProperties_this(); }

	// update
	void Update_this()
	{
		ModelEditor::Update_this();

		// mass
		SimpleGUI::TextBox(m_textEditState_mass, Vec2(8, 8));
		getModel<MoleculeAsset>()->m_mass = Parse<double>(m_textEditState_mass.text);
		MoveDrawPos(0, 48);
	}
	void update() override { Update_this(); }
};
*/