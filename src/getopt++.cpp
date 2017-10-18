#include <getopt++.h>
using namespace GOpp;

Parser::Parser(int argc, char** argv,const std::initializer_list<Command::Definition> CommandParameters):
	Parser(ConvertArgvToVect(argc,argv),std::vector<Command::Definition>(CommandParameters.begin(),CommandParameters.end()))
{
}

Parser::Parser(std::initializer_list<std::string> Parameters,
		const std::initializer_list<Command::Definition> CommandParameters):
	Parser(std::vector<std::string>(Parameters.begin(),Parameters.end()),
			std::vector<Command::Definition>(CommandParameters.begin(),CommandParameters.end()))
{
}

Parser::Parser(std::vector<std::string> Parameters,
		const std::vector<Command::Definition> CommandParameters)
{
	if(Parameters.size() <= 0) {
		throw std::invalid_argument("Parameters");
	} 
	ProgramName = Parameters[0];
	Arguments = Parameters;
	try{
		ArgSymTable = BuildSymTable<std::string,Command::Definition>(CommandParameters,
				[](Command::Definition Def) {return Def.AcceptedArguments; });
		ParSymTable = BuildSymTable<std::string, Command::Definition>(CommandParameters,
				[](Command::Definition Def) {return Def.ValidNames; });
	} catch(std::logic_error){
		throw;
	}
	if(AreSymTablesColliding<std::string>(ArgSymTable,ParSymTable) == true){
		throw std::logic_error("");
	}
	this->Parse(Arguments,std::vector<Command::Definition>(CommandParameters.begin(),CommandParameters.end()));
}

const std::vector<std::string>& Parser::GetArguments(void) const
{
	return Arguments;
}

const std::vector<std::shared_ptr<Command>> Parser::GetCommands(void) const
{
	return Commands;
}

const std::string Parser::GetProgramName(void) const
{
	return ProgramName;
}

bool Parser::operator[](std::string Argument) const
{
	//Was given 'Argument' used
	for(int i = 0 ; i < Commands.size() ; i++){
		if(Commands[i]->Properties ==  Argument ){
			return true;
		}
	}
	return false;
}

bool Parser::AddCommand(std::string Argument,int Indice, std::vector<Command::Definition> CommandParameters)
{
	for(int b = 0 ; b < CommandParameters.size() ; b++){
		if(CommandParameters[b] == Argument){
			std::shared_ptr<Command> Temp(new Command(Indice,CommandParameters[b]));
			this->Commands.push_back(Temp);
			return true;
		}
	}
	return false;
}

void Parser::Parse(std::vector<std::string> Argv,std::vector<Command::Definition> CommandParameters)
{
	for(int a = 1;a<Argv.size();a++){
		if(LastUsedCommand != nullptr && (LastUsedCommand->HasArgument(Argv[a]) ||
					LastUsedCommand->Properties.MaxArgCount == -1) && ParSymTable.find(Argv[a]) == ParSymTable.end()) {
			//If Argv[a] is not valid name, then it might be an argument to some preexisting function
			LastUsedCommand->AddArgument(Argv[a]);
		} else if(ParSymTable.find(Argv[a]) != ParSymTable.end()){
			this->AddCommand(Argv[a],a,CommandParameters);
			LastUsedCommand = Commands.back();
		} else{
			throw std::invalid_argument(Argv[a]);
		}
	}
	for(int b  = 0 ; b<Commands.size() ; b++){
		if(Commands[b]->HasValidArgCount() == false){
			throw std::invalid_argument("");
		}
	}
}


const Command::Definition& Command::GetProperties(void) const
{
	return Properties;
}

const int& Command::GetIndex(void) const
{
	return Index;
}

const std::vector<std::string>& Command::GetArguments(void) const
{
	return Arguments;
}

Command::Command(int index,Command::Definition Parameters):
		Index(index),Properties(Parameters)
{
}

bool Command::operator[](std::string Argument) const
{
	//Was given 'Argument' used?
	for(int i = 0 ; i < Arguments.size() ; i++){
		if(Arguments[i] == Argument){
			return true;
		}
	}
	return false;
}

bool Command::operator==(std::string Argument) const
{
	return Properties ==  Argument;
}

bool Command::AddArgument(std::string Argument)
{
	if(Properties.MaxArgCount > 0){
		for(int c = 0 ; c < Properties.AcceptedArguments.size() && 
			Arguments.size() < Properties.MaxArgCount; c++){
			if(Argument == Properties.AcceptedArguments[c]){
				Arguments.push_back(Argument);
				return true;
			}
		}
			
	} else if(Properties.MaxArgCount == -1){
		Arguments.push_back(Argument);
		return true;
	}
	return false;
}

bool Command::HasValidArgCount(void) const
{
	return (Properties.MaxArgCount == -1 || Arguments.size() <= Properties.MaxArgCount)
		&& Arguments.size() >= Properties.MinArgCount;
}

bool Command::HasArgument(const std::string& CandArg) const
{
	return std::find(Properties.AcceptedArguments.begin(),Properties.AcceptedArguments.end(),CandArg)
		!= Properties.AcceptedArguments.end()/* || ((Properties.Flags & NoLimit) != false)*/;
}

bool Command::HasName(const std::string& CandName) const
{
	return std::find(Properties.ValidNames.begin(),Properties.ValidNames.end(),CandName) != Properties.ValidNames.end();
}

bool Command::Definition::operator==(std::string Argument) const
{
	return std::find(ValidNames.begin(),ValidNames.end(),Argument) != ValidNames.end();
}

std::vector<std::string> GOpp::ConvertArgvToVect(int argc,char **argv)
{
	if(argv == nullptr || argc < 1) {
		throw std::invalid_argument("Argv");
	} 
	std::vector<std::string> ArgList;
	for(int i = 0;i<argc;i++){
		if(argv[i] == nullptr){
			throw std::invalid_argument("nullptr");
		}
		std::string temp(argv[i]);
		ArgList.push_back(temp);
	}
	return ArgList;
}

