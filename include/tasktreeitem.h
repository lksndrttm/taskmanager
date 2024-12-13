#pragma once
#include "TargetList.h"
#include <QList>

class TreeItem
  {
  public:
      explicit TreeItem(std::map<string, string> trg, TreeItem *parentItem = nullptr);
      ~TreeItem();

      void appendChild(TreeItem *child);

      TreeItem *child(int row);
      int childCount() const;
      int columnCount() const;
      QVariant data(int column) const;
      int row() const;
      TreeItem *parentItem();

  private:
      QList<TreeItem*> m_childItems;
      ITargetPtr m_itemData;
      TreeItem *m_parentItem;
  };
