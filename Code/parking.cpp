void parking()
{
  while(1)
  {
    if (getInSensor() != mlastInSensor)
	{
		if (getInSensor())
		{
			if (mCounter < mCapacity)
			{
				openGate();
		
				mIsOpen = true;
			}
		}
		else
		{
			if (mIsOpen)
			{
				mCounter++;

				closeGate();
				
				mIsOpen = false;
			}
		}

		mlastInSensor = getInSensor();
	}

	if (getOutSensor() != mlastOutSensor)
	{
		if (!getOutSensor())
		{
			mCounter--;
		}

		mlastInSensor = getOutSensor();
	}
}