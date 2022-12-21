# SimpleVector

Собсвенная реализация контейнера вектора(std::vector). В качестве хранилища используется умный казатель на массив в динамической памяти ArrayPtr.
Создан в учебных целях при поддержке команды Яндекс.Практикума(2022год). Доработка и развитие не планируются.

# Реализованные методы.

1. Конструкторы:
  - SimpleVector(size_t size)
  - SimpleVector(size_t size, const Type& value)
  - SimpleVector(size_t size, Type&& value) 
  - SimpleVector(std::initializer_list<Type> init)
  - SimpleVector(const SimpleVector& other)
  - SimpleVector(SimpleVector&& other)
2. Итераторы begin, cbegin, end, cend
3. Операторы сравнения векторов  
4. Методы:
  - void Reserve(size_t new_capacity)
  - size_t GetSize()
  - void Clear()
  - bool IsEmpty()
  - void Resize()
  - void PushBack(const Type& item)
  - void PushBack(Type&& item)
  - Iterator Insert(ConstIterator pos, const Type& value)
  - Iterator Insert(ConstIterator pos, Type&& value)
  - void PopBack()
  - Iterator Erase(ConstIterator pos)
  - Iterator Erase(Iterator pos)
  
  
# Системные требования.

С++17


