// Knapsack class
// Version f08.1

class knapsack
{
   public:
      knapsack(ifstream &fin);
      knapsack(const knapsack &);
      int getCost(int) const;
      int getValue(int) const;
      int getCost() const;
      int getValue() const;
      int getNumObjects() const;
      int getCostLimit() const;
      void printSolution();
      void select(int);
      void unSelect(int);
      bool isSelected(int) const;
      pair<double, bool> bound();
      void setNum(int);
      int getNum() const;
      void setValuePerUnitCost();

   private:
      int numObjects;
      int costLimit;
      int num;
      vector<int> value;
      vector<int> cost;
      vector<bool> selected;
      int totalValue;
      int totalCost;
      vector<double> valuePerUnitCost;
};

knapsack::knapsack(ifstream &fin)
// Construct a new knapsack instance using the data in fin.
{
   int n, b, j, v, c;
   
   fin >> n;  // read the number of objects
   fin >> b;  // read the cost limit

   numObjects = n;
   costLimit = b;
   
   value.resize(n);
   cost.resize(n);
   selected.resize(n);
   
   for (int i = 0; i < n; i++)
   {
      fin >> j >> v >> c;
      value[j] = v;
      cost[j] = c;
      unSelect(j);
   }

   totalValue = 0;
   totalCost = 0;
   num = 0;
   setValuePerUnitCost();
}

knapsack::knapsack(const knapsack &k)
// Knapsack copy constructor.
{
   int n = k.getNumObjects();
   
   value.resize(n);
   cost.resize(n);
   selected.resize(n);
   numObjects = k.getNumObjects();
   costLimit = k.getCostLimit();

   totalCost = 0;
   totalValue = 0;
   num = k.getNum();

   for (int i = 0; i < n; i++)
   {
      value[i] = k.getValue(i);
      cost[i] = k.getCost(i);
      if (k.isSelected(i))
	 select(i);
      else
	 unSelect(i);
   }
   setValuePerUnitCost();
}

int knapsack::getNumObjects() const
{
   return numObjects;
}

int knapsack::getCostLimit() const
{
   return costLimit;
}

void knapsack::setValuePerUnitCost()
{
	for (int i = 0; i < numObjects; i++) {
		valuePerUnitCost.push_back(value[i] / cost[i]);
	}
}


int knapsack::getValue(int i) const
// Return the value of the ith object.
{
   if (i < 0 || i >= getNumObjects())
      throw rangeError("Bad value in knapsack::getValue");
   
   return value[i];
}

int knapsack::getCost(int i) const
// Return the cost of the ith object.
{
   if (i < 0 || i >= getNumObjects())
      throw rangeError("Bad value in knapsack::getCost");

   return cost[i];
}

int knapsack::getCost() const
// Return the cost of the selected objects.
{
   return totalCost;
}

int knapsack::getValue() const
// Return the value of the selected objects.
{
   return totalValue;
}

ostream &operator<<(ostream &ostr, const knapsack &k)
// Print all information about the knapsack.
{
   cout << "------------------------------------------------" << endl;
   cout << "Num objects: " << k.getNumObjects() << " Cost Limit: " << k.getCostLimit() << endl;

   int totalValue = 0;
   int totalCost = 0;
   
   for (int i = 0; i < k.getNumObjects(); i++)
   {
      totalValue += k.getValue(i);
      totalCost += k.getCost(i);
   }

   cout << "Total value: " << totalValue << endl;
   cout << "Total cost: " << totalCost << endl << endl;

   for (int i = 0; i < k.getNumObjects(); i++)
      cout << i << "  " << k.getValue(i) << " " << k.getCost(i) << endl;

   cout << endl;

   return ostr;
}

void knapsack::printSolution()
// Prints out the solution.
{
   cout << "------------------------------------------------" << endl;

   cout << "Total value: " << getValue() << endl;
   cout << "Total cost: " << getCost() << endl << endl;

   // Print out objects in the solution
   for (int i = 0; i < getNumObjects(); i++)
      if (isSelected(i))
	 cout << i << "  " << getValue(i) << " " << getCost(i) << endl;

   cout << endl;
}

ostream &operator<<(ostream &ostr, vector<bool> v)
// Overloaded output operator for vectors.
{
   for (int i = 0; i < v.size(); i++)
      cout << v[i] << endl;

   return ostr;
}

void knapsack::select(int i)
// Select object i.
{
   if (i < 0 || i >= getNumObjects())
      throw rangeError("Bad value in knapsack::Select");

   if (selected[i] == false)
   {
      selected[i] = true;
      totalCost = totalCost + getCost(i);
      totalValue = totalValue + getValue(i);
   }
}

void knapsack::unSelect(int i)
// unSelect object i.
{
   if (i < 0 || i >= getNumObjects())
      throw rangeError("Bad value in knapsack::unSelect");

   if (selected[i] == true)
   {
      selected[i] = false;
      totalCost = totalCost - getCost(i);
      totalValue = totalValue - getValue(i);
   }
}

bool knapsack::isSelected(int i) const
// Return true if object i is currently selected, and false otherwise.
{
   if (i < 0 || i >= getNumObjects())
      throw rangeError("Bad value in knapsack::getValue");

   return selected[i];
}

//sets the current index to be worked on the knapsack subproblem
void knapsack::setNum(int i)
{
	num = i;
}

//gets the current index to be worked on the knapsack subproblem
int knapsack::getNum() const
{
	return num;
}

//returns a pair for the bound whee the double is the bound value and the bool is true if the knapsack is an integral solution
pair<double, bool> knapsack::bound()
{
	pair<double, bool> result;
	
	//gets a local value of the first index to look at for a bound
	int localNum = num;
	
	//used as a local variable of the most valuable object not selected
	int bestIndex;
	
	//used if we need to take a fractional value of an object to fill the knapsack when calculating the bound
	double fractionalValue;
	
	//used to determine if a solution is integral
	bool isIntegral = true;
	while (totalCost < costLimit) {
		//if we have incremented through the list, stop checking
		if (localNum >= numObjects) {
			break;
		}
		
		//set the best to be the first element not decided on, then check if any are better
		bestIndex = localNum;
		for(int i = localNum; i < numObjects; i++) {
			if (valuePerUnitCost[i] > valuePerUnitCost[bestIndex] && false == selected[i]) {
				bestIndex = i;
			}
		}
		
		//select the best valued item
		select(bestIndex);
		
		//if our knapsack is now too full, we must take a fraction, then brak the loop since we have our best bound
		if(totalCost > costLimit) {
			unSelect(bestIndex);
			int remainingWeight = costLimit - totalCost;
			fractionalValue = remainingWeight / cost[bestIndex] * value[bestIndex];
			break;
		}
		
		//if the most valuable item was the first item checked, increment our local variable so we start at the next item when looping
		if (bestIndex == localNum) {
			localNum++;
		}
	}
	
	result.first = totalValue + fractionalValue;
	
	//unselect the items that were selected for the bound
	//if any item was selected (fully), our solution is not integral since more items can be selected into the knapsack
	for (int i = num+1; i < numObjects; i++) {
		if(isSelected(i)){
			unSelect(i);
			isIntegral = false;
		}
	}
	
	result.second = isIntegral;
	
	return result;
}

