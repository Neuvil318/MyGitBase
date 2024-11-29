#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <algorithm>
#include <map>
#include <set>

using namespace std;
class Reminder;
class Task {
private:
	int id;
	string Title;
	string Description;
	time_t StartTime;
	time_t EndTime;
	bool isCompleted;
	bool isImportant;
	set<string> tags;
	bool hasReminder;
	Reminder* reminder;
public:
	Task(int _id, string _Title, string _Description, time_t _StartTime, time_t _EndTime,
		bool _isCompleted, bool _isImportant, set<string> _tags, bool _hasReminder, Reminder* _reminder) :
		id(_id), Title(_Title), Description(_Description), StartTime(_StartTime), EndTime(_EndTime),
		isCompleted(_isCompleted), isImportant(_isImportant),
		tags(_tags), hasReminder(_hasReminder), reminder(_reminder) {}

	int GetID() {
		return id;
	}
	string GetTitle() {//获取title
		return Title;
	}
	int ResetTitle(string newtitle) {//更改title，非空返回1，为空返回0
		if (newtitle.empty())return 0;
		Title = newtitle;
		return 1;
	}
	string GetDescription() {
		return Description;
	}
	int ResetDescription(string newdescription) {//更改description，非空返回1，为空返回0
		if (newdescription.empty())return 0;
		Description = newdescription;
		return 1;
	}
	void SetPrio(bool prio) {
		isImportant = prio;
	}
	void AddTags(string newtag) {
		tags.insert(newtag);
	}
	void printTaskDetails(bool print_tag) const {//打印详细信息，print_tag为1则输出其所有tag

		char stime[26];
		char etime[26];
		ctime_s(stime, sizeof(stime), &StartTime);
		ctime_s(etime, sizeof(etime), &EndTime);
		cout << "\nID:" << id
			<< "\nTitle: " << Title << "\nDescription: " << Description
			<< "\nStart: " << stime << "End: " << etime << "Priority: "
			<< (isImportant == 1 ? "High" : "Low")
			<< "\nisCompleted: " << (isCompleted == 1 ? "YES" : "NO");
		if (print_tag == 1) {
			cout << "\nTags: ";
			for (set<string>::iterator it = tags.begin(); it != tags.end(); ++it) {
				cout << *it << " ";
			}
		}
		cout << endl;
	}
	int DeleteTags(string tag) {//删除指定tag，成功返回1，失败返回0
		if (!tags.erase(tag))return 0;
		else return 1;
	}

	void markAsCompleted() {//设置任务已完成
		isCompleted = 1;
	}
	time_t Getstarttime()const
	{
		return StartTime;

	}
	time_t Getendtime() const {
		return EndTime;
	}

	Reminder* Getreminder() const {
		return reminder;

	}
	void reset_hasrem(bool bl) {
		hasReminder = bl;
	}
};

class Tag {
private:
	string tagname;
	set<Task*>tasks;

public:
	Tag(string _tagname, set<Task*>_tasks) :
		tagname(_tagname), tasks(_tasks) {}

	int addtask(Task* newtask) {
		bool inserted = tasks.insert(newtask).second;
		if (inserted)return 1;
		else
			return 0;
	}
	void print_tasks()
	{
		for (set<Task*>::iterator it = tasks.begin(); it != tasks.end(); ++it) {
			(*it)->printTaskDetails(0);
		}

	}
	int deletetask(Task* taskname) {
		if (!tasks.erase(taskname))return 0;
		else return 1;
	}
	string Gettagname()
	{
		return tagname;

	}
	bool SearchTask(Task* taskname) {//查询某个task是否在Tag中
		return bool(tasks.count(taskname));
	}
};

class Reminder {
private:
	Task* task;
	time_t Rem_time;
	bool state;//闹钟是否开
public:
	Reminder(Task* _task, time_t _Rem_time, bool _state) :
		task(_task), Rem_time(_Rem_time), state(_state) {}


	void SendMessage() {
		//TODO
		cout << task->GetTitle() << " is ready to be done! \n more information: "
			<< task->GetDescription() << endl;
	}
	void checkclock() {//如果闹钟状态为开 且当前时间已超过设定闹钟时间 则发送提醒
		if (state) {
			time_t curtime;
			time(&curtime);
			if (curtime >= Rem_time) {
				SendMessage();
				state = 0;
			}
		}
	}
	void reset_state(bool newstate) {
		state = newstate;
		checkclock();
	}
	int Reset_Rem_time(time_t newtime) {
		time_t task_stime = task->Getstarttime();
		time_t task_etime = task->Getendtime();
		if (newtime > task_etime || newtime < task_stime) {
			cout << "illegal time！ Please try again.\n";
			return 0;
		}
		Rem_time = newtime;
		reset_state(1);
		checkclock();
		return 1;
	}


	int set_rem(Task* newtask, time_t newtime, bool newstate)
	{
		task = newtask;
		time_t task_stime = task->Getstarttime();
		time_t task_etime = task->Getendtime();
		if (newtime > task_etime || newtime < task_stime) {
			cout << "illegal time！ Please try again.\n";
			return 0;
		}
		Rem_time = newtime;
		state = newstate;
		checkclock();
		return 1;

	}
};

class User {
private:
	string Username;
	vector<Task*>tasks;
	set<Tag*>tags;
	vector<Task*>done_tasks;
	int id = 0;
public:
	User(string username) : Username(username) {}

	void add_new_task(string Title, string Description, time_t StartTime, time_t EndTime,
		bool isCompleted, bool isImportant)
	{
		set<string>tag = {};
		bool hasReminder = 0;
		Reminder rem(NULL, 0, 0);
		Task* newtask = new Task(id, Title, Description, StartTime, EndTime,
			isCompleted, isImportant, tag, hasReminder, &rem);
		id++;
		time_t curtime;
		time(&curtime);
		rem.set_rem(newtask, curtime, 0);
		tasks.push_back(newtask);
		//cout << "done" << endl;
		//newtask.printTaskDetails(1);
	}

	void add_new_tag(string tagstr)//创建一个新tag
	{
		set<Task*>tasks = {};
		Tag* newtag = new Tag(tagstr, tasks);
		tags.insert(newtag);

	}
	void add_tag_to_task(Tag* tag, Task* task)//为task添加tag
	{
		string tagstr = tag->Gettagname();
		task->AddTags(tagstr);
		tag->addtask(task);
	}
	int set_reminder(Task* task, time_t newtime) {//设置闹钟时间
		Reminder* rem = task->Getreminder();
		int pt = rem->Reset_Rem_time(newtime);
		if (pt == 0)return 0;
		else {
			task->reset_hasrem(1);
		}
	}

	void sortTasksByEndtime() {
		std::sort(tasks.begin(), tasks.end(), [](const Task* a, const Task* b) {
			return a->Getendtime() < b->Getendtime();
			});
	}

	void printAllTasks() const {
		cout << "\nTasks for User: " << Username << "\n";
		for (int i = 0; i < tasks.size(); ++i) {
			tasks[i]->printTaskDetails(1);
		}
	}

	void printAllTags() const {
		cout << "\nTags for User: " << Username << "\n";
		for (set<Tag*>::iterator it = tags.begin(); it != tags.end(); ++it) {
			Tag* t = *it;
			string str = t->Gettagname();
			cout << str << " ";
		}
		cout << endl;
	}

	void printCompletedTasks() const {
		cout << "\nCompleted Tasks for User: " << Username << "\n";
		for (int i = 0; i < done_tasks.size(); ++i) {
			done_tasks[i]->printTaskDetails(1);
		}
	}

	void markTaskAsCompleted(int taskId) {
		for (int i = 0; i < tasks.size(); ++i) {
			//cout << "\n\n\n" << tasks[i]->GetID() << "\n\n\n\n\n";
			if (tasks[i]->GetID() == taskId) {
				tasks[i]->markAsCompleted();
				done_tasks.push_back(tasks[i]);
				return;
			}
		}
		cout << "Task not found!\n";
	}

	string GetUsername() const {
		return Username;
	}

	Task** GetTasks() {
		return &tasks[0];
	}
	Task* FindTask(int id) {
		for (vector<Task*>::iterator it = tasks.begin(); it != tasks.end(); ++it) {
			Task* t = *it;
			if (t->GetID() == id)return t;
		}
		return nullptr;

	}

	Tag* FindTag(string tagname) {
		for (set<Tag*>::iterator it = tags.begin(); it != tags.end(); ++it) {
			Tag* t = *it;
			//cout << endl << endl << t->Gettagname() << endl << endl;
			if (t->Gettagname() == tagname)return t;
		}
		return nullptr;
	}

};

// 显示主菜单
void displayMenu() {
	cout << "\n===== Task Manager Menu =====\n";
	cout << "1. Create New Task\n";
	cout << "2. View All Tasks\n";
	cout << "3. Mark Task as Completed\n";
	cout << "4. Create New Tag\n";
	cout << "5. Assign Tag to Task\n";
	cout << "6. Set Task Reminder\n";
	cout << "7. view all tags\n";
	cout << "8. Query tasks with a specific tag\n";
	cout << "9. Exit\n";
	cout << "=============================\n";
}

int main() {
	string username;
	cout << "Enter your username: ";
	cin >> username;

	User user(username);
	cout << "Welcome, " << username << "!\n";
	while (true) {
		displayMenu();
		int choice;
		cout << "Enter your choice: ";
		cin >> choice;

		switch (choice) {
		case 1: {
			string title, description;
			cout << "Enter Task Title: ";
			cin.ignore();
			getline(cin, title);
			cout << "Enter Task Description: ";
			getline(cin, description);

			time_t now = time(0);

			int year, month, day, hour, minute;
			cout << "Enter End year: ";
			cin >> year;
			cout << "Enter End month: ";
			cin >> month;
			cout << "Enter End day: ";
			cin >> day;
			cout << "Enter End hour: ";
			cin >> hour;
			cout << "Enter End minute: ";
			cin >> minute;
			std::tm t = {};
			t.tm_year = year - 1900;  // tm_year 从1900年开始
			t.tm_mon = month - 1;     // tm_mon 从0开始
			t.tm_mday = day;
			t.tm_hour = hour;
			t.tm_min = minute;
			t.tm_sec = 0;
			time_t end_time = std::mktime(&t);

			//time_t end_time = now + 3600;
			user.add_new_task(title, description, now, end_time, false, false);
			cout << "Task Created Successfully!" << endl;
			break;
		}
		case 2: {
			user.printAllTasks();
			break;
		}
		case 3: {
			int taskId;
			cout << "Enter Task ID to mark as completed: ";
			cin >> taskId;
			user.markTaskAsCompleted(taskId);
			cout << "Task marked as completed!" << endl;
			break;
		}
		case 4: {
			string tag_name;
			cout << "Enter Tag Name: ";
			cin >> tag_name;
			user.add_new_tag(tag_name);
			cout << "Tag created successfully!" << endl;
			break;
		}
		case 5: {
			string tag_name;
			int task_id;
			cout << "Enter Task ID: ";
			cin >> task_id;


			// Find task by ID
			Task* task = nullptr;

			task = user.FindTask(task_id);

			if (task != nullptr) {
				// Find the tag
				cout << "Enter Tag Name: ";
				cin >> tag_name;

				Tag* tag = user.FindTag(tag_name);

				if (tag != nullptr) {
					user.add_tag_to_task(tag, task);
					cout << "Tag added to task successfully!" << endl;
				}
				else {
					cout << "Tag not found!" << endl;
				}
			}
			else {
				cout << "Task not found!" << endl;
			}
			break;
		}
		case 6: {
			int task_id;
			time_t reminder_time;
			cout << "Enter Task ID to set reminder: ";
			cin >> task_id;
			cout << "Enter reminder time \n ";

			int year, month, day, hour, minute;
			cout << "Enter reminder year: ";
			cin >> year;
			cout << "Enter reminder month: ";
			cin >> month;
			cout << "Enter reminder day: ";
			cin >> day;
			cout << "Enter reminder hour: ";
			cin >> hour;
			cout << "Enter reminder minute: ";
			cin >> minute;
			std::tm t = {};
			t.tm_year = year - 1900;  // tm_year 从1900年开始
			t.tm_mon = month - 1;     // tm_mon 从0开始
			t.tm_mday = day;
			t.tm_hour = hour;
			t.tm_min = minute;
			t.tm_sec = 0;
			reminder_time = std::mktime(&t);


			Task* task = nullptr;

			task = user.FindTask(task_id);

			if (task != nullptr) {
				int result = user.set_reminder(task, reminder_time);
				if (result == 1) {
					cout << "Reminder set successfully!" << endl;
				}
				else {
					cout << "Failed to set reminder!" << endl;
				}
			}
			else {
				cout << "Task not found!" << endl;
			}
			break;
		}
		case 7: {
			user.printAllTags();
			break;
		}
		case 8: {
			string str;
			cout << "Enter Tag Name: ";
			cin >> str;
			Tag* t = user.FindTag(str);
			if (t == nullptr) {
				cout << "Tag not found!" << endl;
			}
			else {
				t->print_tasks();
				cout << "task print successfully!" << endl;
			}
			break;
		}
		case 9: {
			cout << "Exiting the Task Manager... Goodbye!" << endl;
			return 0;
		}

		default:
			cout << "Invalid choice. Please try again." << endl;
			break;
		}
	}
}