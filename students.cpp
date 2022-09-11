#include <iostream>
#include <ctime>
#include <string>
#include <list>
#include <vector>
#include <map>
#include <algorithm>
using namespace std;

class Group;

class StudentAbstract {
public:
	virtual void show()=0;
	virtual void setGroup(Group*)=0;
	virtual Group* getGroup()=0;
	virtual ~StudentAbstract() {
	}
};

class Student: public StudentAbstract {
	string name;
	Group *group;
public:
	Student(string name, Group *group) {
		this->name = name;
		this->group = group;
	}
	Student(string name) :
			Student(name, nullptr) {
	}
	Group* getGroup() override {
		return this->group;
	}
	void setGroup(Group *g) override {
		this->group = g;
	}
	void show() override; // Пришлось перенести после декларации Group
};

class StudentHWDecorator: public StudentAbstract {
protected:
	StudentAbstract *student;
public:
	StudentHWDecorator(StudentAbstract *student) {
		this->student = student;
	}
	Group* getGroup() override {
		return this->student->getGroup();
	}
	void setGroup(Group *g) override {
		this->student->setGroup(g);
	}
	virtual void show()=0;
	virtual ~StudentHWDecorator() {
	}
};

class Specialist: public StudentHWDecorator {
public:
	Specialist(StudentAbstract *student) :
			StudentHWDecorator(student) {
	}
	void show() override {
		cout << "Specialist student ";
		this->student->show();
	}
};

class Multimillionaire: public StudentHWDecorator {
public:
	Multimillionaire(StudentAbstract *student) :
			StudentHWDecorator(student) {
	}
	void show() override {
		cout << "Multimillionaire ";
		this->student->show();
	}
};

class Group {
protected:
	list<StudentAbstract*> students;
	string name;
	Group(string name) {
		this->name = name;
	}
public:
	void addStudent(StudentAbstract *s) {
		if (s->getGroup() != nullptr) {
			s->getGroup()->removeStudent(s);
		}
		s->setGroup(this);
		students.push_back(s);
	}
	void removeStudent(StudentAbstract *s) {
		students.erase(find(students.begin(), students.end(), s));
	}
	void listStudents() {
		for (auto s : students) {
			s->show();
			cout << endl;
		}
	}
	string getName() {
		return this->name;
	}
	virtual void show()=0;
	virtual ~Group() {
	}
	friend class CreateSingletonGroup;
};

void Student::show() {
	cout << this->name << ", from ";
	this->group->show();
}

class GroupDevelopers: public Group {
	GroupDevelopers(string name) :
			Group(name) {
	}
public:
	void show() override {
		cout << "Software Development group " << this->name;
	}
	friend class CreateSingletonGroup;
};

class GroupSysadmins: public Group {
	GroupSysadmins(string name) :
			Group(name) {
	}
public:
	void show() override {
		cout << "System Administration group " << this->name;
	}
	friend class CreateSingletonGroup;
};

class GroupIntroduction: public Group {
	GroupIntroduction(string name) :
			Group(name) {
	}
public:
	void show() override {
		cout << "Introduction to IT group " << this->name;
	}
	friend class CreateSingletonGroup;
};

class CreateSingletonGroup {
public:
	static map<string, Group*> groups;
	CreateSingletonGroup() {
	}
	void createGroup(string name, int type) {
		if (groups.find(name) == groups.end()) {
			switch (type) {
			case 1:
				groups.insert(make_pair(name, new GroupDevelopers(name)));
				break;
			case 2:
				groups.insert(make_pair(name, new GroupSysadmins(name)));
				break;
			default:
				groups.insert(make_pair(name, new GroupIntroduction(name)));
			}
		}
	}
	void listGroups() {
		for (auto g : CreateSingletonGroup::groups) {
			g.second->show();
			cout << endl;
		}
	}
};

map<string, Group*> CreateSingletonGroup::groups;

int main() {
	srand(time(NULL));
	enum groups {
		INTRO, DEVEL, ADMIN
	};
	CreateSingletonGroup cr;
	cr.createGroup("I101", INTRO);
	cr.createGroup("I102", INTRO);
	cr.createGroup("I111", INTRO);
	cr.createGroup("A101", ADMIN);
	cr.createGroup("A112", ADMIN);
	cr.createGroup("D201", DEVEL);
	cr.createGroup("D214", DEVEL);
	cr.createGroup("D131", DEVEL);
	cr.createGroup("D255", DEVEL);

	vector<string> groupnames;
	for (auto g : CreateSingletonGroup::groups)
		groupnames.push_back(g.first);

	//cr.listGroups();

	vector<StudentAbstract*> students;
	students.push_back(new Student("Alice"));
	students.push_back(new Student("Bob"));
	students.push_back(new Student("Charles"));
	students.push_back(new Student("Deborah"));
	students.push_back(new Student("Eve"));
	students.push_back(new Student("Finn"));
	students.push_back(new Student("George"));
	students.push_back(new Student("Hannah"));
	students.push_back(new Student("Ian"));
	students.push_back(new Student("Jake"));
	students.push_back(new Student("Kelly"));
	students.push_back(new Student("Liam"));
	students.push_back(new Student("Michael"));
	students.push_back(new Student("Neal"));
	students.push_back(new Student("Oprah"));
	students.push_back(new Student("Peter"));
	students.push_back(new Student("Quentin"));
	students.push_back(new Student("Richard"));
	students.push_back(new Student("Sean"));
	students.push_back(new Student("Tammy"));
	students.push_back(new Student("Ulysses"));
	students.push_back(new Student("Valentine"));
	students.push_back(new Student("William"));
	students.push_back(new Student("Xavier"));
	students.push_back(new Student("Yvonne"));
	students.push_back(new Student("Zachary"));

	for (auto &s : students) {
		if (rand() % 4 == 0)
			s = new Specialist(s);
		if (rand() % 10 == 0)
			s = new Multimillionaire(s);
		size_t i = rand() % CreateSingletonGroup::groups.size();
		CreateSingletonGroup::groups[groupnames[i]]->addStudent(s);
	}

//	for (auto g : CreateSingletonGroup::groups) {
//		cout << "\t";
//		g.second->show();
//		cout << ":" << endl;
//		g.second->listStudents();
//	}

	for (auto s : students) {
		s->show();
		cout << endl;
	}
	cout << "---------------" << endl;
	for (auto it = students.begin();
			it
					!= partition(students.begin(), students.end(),
							[](StudentAbstract *s) {
								return s->getGroup()->getName() == "I111";
							}); it++) {
		(*it)->show();
		cout << endl;
	}

	return 0;
}
