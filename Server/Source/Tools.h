/******************************************************************************/
bool Validate(RakNet::RakString input, RakNet::RakString &output) {
	if (input.ContainsNonprintableExceptSpaces()) {
		output = RakNet::RakString("Your login or password contains invalid characters.");
		return false;
	}
	if (input.GetLength() < 4) {
		output = RakNet::RakString("Your login or password is too short.");
		return false;
	}
	if (input.GetLength() >= 12) {
		output = RakNet::RakString("Your login or password is too long.");
		return false;
	}
	return true;
}