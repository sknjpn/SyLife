#pragma once

#include "EasyViewer.h"

class EditorViewer : public EasyViewer
{
public:
	void init() override;
	void update() override;

	class AssetList : public EasyViewer
	{
		class Item : public EasyViewer
		{
		public:
			void init() override;
			void update() override;
		};

	public:
		void init() override;
		void update() override;
	};
};

