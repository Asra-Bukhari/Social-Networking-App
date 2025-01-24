#include <iostream>
#include <fstream>
#include <string>
using namespace std;


class Post;
class comments;
class object;


class Helper
{
public:
    static int strLength(const char* str)
    {
        int l = 0;
        while (str[l] != '\0')
        {
            l++;
        }
        return l;
    }

    static char* GetStringFromBuffer(const char* temp)
    {
        int l = strLength(temp);
        char* str = 0;

        if (l > 0)
        {
            str = new char[l + 1];

            for (int i = 0; i < l; i++)
            {
                str[i] = temp[i];
            }

            str[l] = '\0';
        }

        return str;
    }

    static bool compStr(char* str, const char* toSearch)
    {
        int l1 = strLength(str);
        int l2 = strLength(toSearch);

        if (l1 != l2)
            return false;

        for (int i = 0; i < l1; i++)
        {
            if (str[i] != toSearch[i])
                return false;
        }

        return true;
    }

    static char* IdGenerator(const char* initial, int& no)
    {
        int initialLength = strLength(initial);
        no++;                  //for increment in controller AllPosts/AllComments
        int noDigits = 0;
        int temp = no;
        while (temp != 0) {
            temp /= 10;
            noDigits++;
        }
        int size = initialLength + noDigits + 1;
        char* id = new char[size];

        sprintf_s(id, size, "%s%d", initial, no);
        return id;
    }

    template <typename T>
    static void deleteArray(T** arr, int size)
    {
        for (int i = 0; i < size; i++)
        {
            if (arr[i])
                delete arr[i];
        }

        if (arr)
            delete[] arr;
    }
};

class Date {
private:
    int day;
    int month;
    int year;

public:
    static Date currentDate;

    Date(int d = 0, int m = 0, int y = 0)
    {
        day = d;
        month = m;
        year = y;
    }
    Date(const Date& curr)
    {
        day = curr.day;
        month = curr.month;
        year = curr.year;
    }
    bool isDefault()
    {
        return ((day == 0) && (month == 0) && (year == 0));
    }
    void setDate(ifstream& fin)
    {
        fin >> day;
        fin >> month;
        fin >> year;
    }

    static void setCurrentdate(int d, int m, int y)
    {
        currentDate.day = d;
        currentDate.month = m;
        currentDate.year = y;

        cout << "System Date:       " << d << "/" << m << "/" << y << endl;
    }
    bool CompareDate()
    {
        if ((year == currentDate.year && month == currentDate.month && day == currentDate.day) ||
            (year == currentDate.year && month == currentDate.month && (day == currentDate.day - 1 ||
                day == currentDate.day - 2 || day == currentDate.day - 3)))
            return true;

        if ((month == currentDate.month - 1 && ((currentDate.day == 1) && ((day == 30) || (day == 31) || (day == 29)))) ||
            (month == currentDate.month - 1 && ((currentDate.day == 2) && ((day == 30) || (day == 31)))))
            return true;

        if ((month == 12 && ((day == 31) || (day == 30)) && currentDate.month == 1 && currentDate.day == 1 && year == currentDate.year - 1)
            || (month == 12 && (day == 31) && currentDate.month == 1 && currentDate.day == 2 && year == currentDate.year - 1))
            return true;

        return false;
    }
    int GetPostYearGap()
    {
        if ((day == currentDate.day) && (month == currentDate.month) && (year < currentDate.year))
            return (currentDate.year - year);

        return -1;
    }
    void printDate()
    {
        if ((day == currentDate.day) && (month == currentDate.month) && (year == currentDate.year))
            cout << "( 1h )";
        else if (year == currentDate.year && month == currentDate.month && day == currentDate.day - 1)
            cout << "( 1d )";
        else if (year == currentDate.year && month == currentDate.month && day == currentDate.day - 2)
            cout << "( 2d )";
        else if (year == currentDate.year && month == currentDate.month && day == currentDate.day - 3)
            cout << "( 3d )";
        else
            cout << "( " << day << "/" << month << "/" << year << " )";
    }
};

Date Date::currentDate;

class PostContent {

public:
    virtual void ReadDataFromFile(ifstream& fin) {}
    virtual void print() {}
    virtual ~PostContent() {}
};
class Activity : public PostContent
{
private:
    int type;
    char* value;

public:
    Activity()
    {
        type = 0;
        value = 0;
    }
    void ReadDataFromFile(ifstream& fin)
    {
        fin >> type;

        char temp[300];
        fin.getline(temp, 300);
        value = Helper::GetStringFromBuffer(temp);
    }
    void print()
    {
        switch (type)
        {
        case 1:
            cout << " is feeling ";
            break;
        case 2:
            cout << " is thinking about ";
            break;
        case 3:
            cout << " is making ";
            break;
        case 4:
            cout << " is celebrating ";
            break;
        }

        if (value != 0)
            cout << value;
    }
    ~Activity()
    {
        if (value)
            delete[] value;
    }
};

class object {

private:
    char* ID;


protected:
    Post** timeline;
    int postsCount;

public:
    object()
    {
        ID = 0;
        timeline = 0;
        postsCount = 0;
    }
    void setID(const char* id)
    {
        ID = Helper::GetStringFromBuffer(id);
    }
    char* GetID()
    {
        return ID;
    }
    void printID()
    {
        cout << ID;
    }

    virtual void PrintListView() {}
    virtual void printName() {}

    void AddToTimeline(Post*& postptr)
    {
        if ((postsCount == 0) && (!timeline))
            timeline = new Post * [10];
        timeline[postsCount] = postptr;
        postsCount++;
    }

    void ViewTimeline();

    virtual ~object()
    {
        if (ID)
            delete[] ID;

        Helper::deleteArray(timeline, postsCount);
    }
    void view();
};

class comment {

private:
    char* ID;
    char* text;
    object* commentBy;

public:
    comment()
    {
        ID = text = 0;
        commentBy = 0;
    }
    void SetCommentValues(const char* id, const char* txt, object*& prsn)
    {
        ID = Helper::GetStringFromBuffer(id);
        text = Helper::GetStringFromBuffer(txt);
        commentBy = prsn;
    }
    void printCommentDetails()
    {
        cout << "\t\t\t\t";
        commentBy->printName();
        cout << " : " << "\"" << text << "\"" << endl;
    }
    ~comment()
    {
        if (ID)
            delete[] ID;
        if (text)
            delete[] text;
    }
};
class Post
{

private:
    char* ID;
    char* text;
    Date sharedDate;
    object* sharedBy;
    object** likedBy;
    comment** postComments;
    PostContent* content;

    int likesCount;
    int commentsCount;
public:
    Post()
    {
        ID = text = 0;
        sharedBy = 0;
        likedBy = 0;
        postComments = 0;
        content = 0;

        likesCount = 0;
        commentsCount = 0;

    }
    Post(const char* txt, object* userPtr, const char* postId, Date& curr) :sharedDate(curr)
    {
        if (postId)
            ID = Helper::GetStringFromBuffer(postId);
        if (txt)
            text = Helper::GetStringFromBuffer(txt);
        if (userPtr)
            sharedBy = userPtr;

        likedBy = new object * [10];
        postComments = new comment * [10];
        content = 0;

        likesCount = 0;
        commentsCount = 0;
    }
    void ReadDataFromFile(ifstream& fin)
    {
        char temp[100];
        fin >> temp;
        ID = Helper::GetStringFromBuffer(temp);
        sharedDate.setDate(fin);
        fin >> ws;
        fin.getline(temp, 100);
        text = Helper::GetStringFromBuffer(temp);
    }
    Date GetSharedDate()
    {
        return sharedDate;
    }
    void setSharedBy(object*& obj)
    {
        sharedBy = obj;
    }
    void setlikedBy(object*& obj)
    {
        if ((likesCount == 0) && (!likedBy))
            likedBy = new object * [10];
        likedBy[likesCount] = obj;
        likesCount++;
    }

    void AddComment(comment*& newComment)
    {
        if ((commentsCount == 0) && (!postComments))
            postComments = new comment * [10];
        postComments[commentsCount] = newComment;
        commentsCount++;
    }
    virtual void PrintPostDetails();

    char* GetID()
    {
        return ID;
    }
    void ViewLikedList()
    {
        cout << ID << " is Liked By :" << endl;

        if (likesCount == 0)
            cout << "NO LIKES :(" << endl;
        else
        {
            for (int i = 0; i < likesCount; i++)
            {
                likedBy[i]->PrintListView();
            }
        }
    }
    void AddContent(PostContent*& contentPtr)
    {
        content = contentPtr;
    }
    void PrintMemory();
    virtual ~Post()
    {
        if (ID)
            delete[] ID;
        if (text)
            delete[] text;
        if (likedBy)
            delete[] likedBy;

        Helper::deleteArray(postComments, commentsCount);

        if (content)
            delete content;
    }
};


class Page : public object
{
private:
    char* Title;

public:
    Page()
    {
        Title = 0;
    }
    ~Page()
    {
        if (Title)
            delete[] Title;
    }
    void ReadDataFromFile(ifstream& fin)
    {
        char temp[70];
        fin >> temp;
        object::setID(temp);
        fin >> ws;
        fin.getline(temp, 70);
        Title = Helper::GetStringFromBuffer(temp);
    }
    void printName()
    {
        cout << Title;
    }
    void PrintListView()
    {
        object::printID();
        cout << "\t" << Title << endl;
    }
};

class User : public object
{
private:
    char* Fname;
    char* Lname;
    User** FriendList;
    Page** LikedPages;

    int friendCount;
    int likedPgCount;

public:

    User()
    {
        Fname = Lname = 0;
        FriendList = 0;
        LikedPages = 0;

        friendCount = 0;
        likedPgCount = 0;

    }
    ~User()
    {
        if (Fname)
            delete[] Fname;
        if (Lname)
            delete[] Lname;
        if (FriendList)
            delete[] FriendList;
        if (LikedPages)
            delete[] LikedPages;
    }
    void ReadDataFromFile(ifstream& fin)
    {
        char temp[25];
        fin >> temp;
        object::setID(temp);
        fin >> temp;
        Fname = Helper::GetStringFromBuffer(temp);
        fin >> temp;
        Lname = Helper::GetStringFromBuffer(temp);
    }
    void AddFriend(User*& frnd)
    {
        if ((friendCount == 0) && (!FriendList))
            FriendList = new User * [10];
        FriendList[friendCount] = frnd;
        friendCount++;
    }
    void LikePage(Page*& pg)
    {
        if ((likedPgCount == 0) && (!LikedPages))
            LikedPages = new Page * [10];
        LikedPages[likedPgCount] = pg;
        likedPgCount++;
    }
    void printName()
    {
        cout << Fname << " " << Lname;
    }
    void PrintListView()
    {
        object::printID();
        cout << "\t" << Fname << " " << Lname << endl;
    }
    void ViewFriendList()
    {
        cout << Fname << " " << Lname << " - Friend List" << endl << endl;
        if (friendCount == 0)
            cout << "No FRIENDS :(" << endl;
        else
        {
            for (int i = 0; i < friendCount; i++)
            {
                FriendList[i]->PrintListView();
            }
        }
    }
    void ViewLikedPages()
    {
        cout << Fname << " " << Lname << " - Liked Pages" << endl << endl;
        if (likedPgCount == 0)
            cout << "No LIKED PAGES :(" << endl;
        else
        {
            for (int i = 0; i < likedPgCount; i++)
            {
                LikedPages[i]->PrintListView();
            }
        }
    }
    void ViewHome()
    {
        cout << Fname << " " << Lname << " - HomePage" << endl << endl;

        view();

        for (int i = 0; i < friendCount; i++)
        {
            FriendList[i]->view();
        }

        for (int i = 0; i < likedPgCount; i++)
        {
            LikedPages[i]->view();
        }
    }

    void ViewMemory()
    {
        cout << "\nWe hope you enjoy looking back and sharing your memories on Facebook,\nfrom the most recent to those long ago." << endl << endl;
        for (int i = 0; i < postsCount; i++)
        {
            int yearGap = timeline[i]->GetSharedDate().GetPostYearGap();
            if (yearGap != -1)
            {
                cout << "On this Day\n" << yearGap << " Years Ago\n";
                timeline[i]->PrintPostDetails();
            }
        }
    }
};

void Post::PrintPostDetails()
{
    cout << "---";
    sharedBy->printName();
    if (content != 0)
        content->print();
    sharedDate.printDate();
    cout << ":\n\t\t";

    cout << "\"" << text << "\"" << endl;

    for (int i = 0; i < commentsCount; i++)
    {
        postComments[i]->printCommentDetails();
    }
}

void object::ViewTimeline()
{
    printName();
    cout << " - Timeline" << endl << endl;

    for (int i = 0; i < postsCount; i++)
    {
        timeline[i]->PrintPostDetails();
        cout << endl << endl;
    }

}
void object::view()
{
    for (int i = 0; i < postsCount; i++)
    {
        Date temp = timeline[i]->GetSharedDate();
        if (temp.CompareDate())
        {
            timeline[i]->PrintPostDetails();
            cout << endl << endl;
        }

    }
}
void Post::PrintMemory()
{
    cout << "~~~ ";
    sharedBy->printName();
    cout << " shared a memory ~~~ ";
    sharedDate.printDate();
    cout << "\n\t\t\"" << text << "\"" << endl;
}
class memory : public Post
{
private:
    Post* originalPost;

public:
    memory()
    {
        originalPost = 0;
    }
    memory(const char* txt, object* userPtr, Post* postPtr, const char* postId, Date& curr) :Post(txt, userPtr, postId, curr)
    {
        originalPost = postPtr;
    }
    void PrintPostDetails()
    {
        PrintMemory();
        cout << "\t\t\t ~~~ " << (originalPost->GetSharedDate().GetPostYearGap()) << " years ago ~~~" << endl;
        cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
        originalPost->PrintPostDetails();
        cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
    }
    ~memory() {}
};
class Controller {

private:
    User** AllUsers;
    Page** AllPages;
    Post** AllPosts;

    static int totalUsers;
    static int totalPages;
    static int totalPosts;
    static int totalComments;
    static int totalActivities;

public:
    Controller()
    {
        AllUsers = 0;
        AllPages = 0;
        AllPosts = 0;
    }
    ~Controller()
    {
        Helper::deleteArray(AllUsers, totalUsers);
        Helper::deleteArray(AllPages, totalPages);

        if (AllPosts)
            delete[] AllPosts;
    }

    User* SearchUserByID(const char* ID)
    {
        int i = 0;
        while (AllUsers[i] != NULL)
        {
            if (Helper::compStr(AllUsers[i]->GetID(), ID))
                return AllUsers[i];
            i++;
        }

        return NULL;
    }
    Page* SearchPageByID(const char* ID)
    {
        int i = 0;
        while (AllPages[i] != NULL)
        {
            if (Helper::compStr(AllPages[i]->GetID(), ID))
                return AllPages[i];
            i++;
        }

        return NULL;
    }
    Post* SearchPostByID(const char* ID)
    {
        int i = 0;
        while (AllPosts[i] != NULL)
        {
            if (Helper::compStr(AllPosts[i]->GetID(), ID))
                return AllPosts[i];
            i++;
        }

        return NULL;
    }
    object* SearchobjByID(const char* ID)
    {
        object* obj = SearchUserByID(ID);
        if (obj == nullptr)
            obj = SearchPageByID(ID);

        return obj;
    }

    void LoadAllUsers(const char* fileName)
    {
        ifstream fin(fileName);
        if (fin.is_open())
        {
            fin >> totalUsers;
            AllUsers = new User * [totalUsers + 1];
            for (int i = 0; i < totalUsers; i++)
            {
                AllUsers[i] = new User;
                AllUsers[i]->ReadDataFromFile(fin);
            }
            AllUsers[totalUsers] = NULL;

            fin.close();
        }
        else
        {
            cout << "< Cannot Open Users Input File >";
            throw exception();
        }
    }

    void LoadAllPages(const char* fileName)
    {
        ifstream fin(fileName);
        if (fin.is_open())
        {
            fin >> totalPages;
            AllPages = new Page * [totalPages + 1];
            for (int i = 0; i < totalPages; i++)
            {
                AllPages[i] = new Page;
                AllPages[i]->ReadDataFromFile(fin);
            }
            AllPages[totalPages] = NULL;

            fin.close();
        }
        else
        {
            cout << "< Cannot Open Pages Input File >";
            throw exception();
        }
    }

    void  LinkUsersAndPages(const char* fileName)
    {
        ifstream fin(fileName);
        if (fin.is_open())
        {
            char temp[10];
            fin >> temp;

            while (!(Helper::compStr(temp, "-1")))
            {
                char* tempID = Helper::GetStringFromBuffer(temp);
                User* userID = SearchUserByID(tempID);
                delete[] tempID;
                if (userID == NULL)
                    continue;

                fin >> temp;
                while (!(Helper::compStr(temp, "-1")))
                {
                    tempID = Helper::GetStringFromBuffer(temp);
                    User* friendID = SearchUserByID(tempID);
                    if (friendID == NULL)
                        continue;
                    delete[] tempID;

                    userID->AddFriend(friendID);
                    fin >> temp;
                }

                fin >> temp;
                while (!(Helper::compStr(temp, "-1")))
                {
                    tempID = Helper::GetStringFromBuffer(temp);
                    Page* likedPgID = SearchPageByID(tempID);
                    if (likedPgID == NULL)
                        continue;
                    delete[] tempID;

                    userID->LikePage(likedPgID);
                    fin >> temp;
                }

                fin >> temp;
            }

            fin.close();
        }
        else
        {
            cout << "< Cannot Open File for Users Friends and Liked Pages >";
            throw exception();
        }

    }

    void LoadAllPosts(const char* fileName)
    {
        ifstream fin(fileName);
        if (fin.is_open())
        {
            fin >> totalPosts;
            AllPosts = new Post * [totalPosts + 1];

            char temp[25];
            for (int i = 0; i < totalPosts; i++)
            {
                AllPosts[i] = new Post;
                AllPosts[i]->ReadDataFromFile(fin);

                fin >> temp;
                object* obj = SearchobjByID(temp);

                if (obj != nullptr)
                {
                    AllPosts[i]->setSharedBy(obj);
                    obj->AddToTimeline(AllPosts[i]);
                }
                else
                    cout << obj->GetID() << " user/page not found." << endl;


                fin >> temp;
                while (!(Helper::compStr(temp, "-1")))
                {
                    object* obj = SearchobjByID(temp);

                    if (obj != nullptr)
                        AllPosts[i]->setlikedBy(obj);
                    else
                        cout << obj->GetID() << " user/page not found." << endl;

                    fin >> temp;
                }
            }
            AllPosts[totalPosts] = NULL;

            fin.close();
        }
        else
        {
            cout << "< Cannot Open Posts Input File >";
            throw exception();
        }
    }

    void LoadAllComments(const char* fileName)
    {
        ifstream fin(fileName);
        if (fin.is_open())
        {
            int total;
            fin >> total;

            char commentID[25];
            char temp[300];
            char* tempPtr;
            for (int i = 0; i < total; i++)
            {
                comment* commentPtr = new comment;

                fin >> commentID;

                fin >> temp;
                tempPtr = Helper::GetStringFromBuffer(temp);
                Post* postPtr = SearchPostByID(tempPtr);
                delete[] tempPtr;

                fin >> temp;
                tempPtr = Helper::GetStringFromBuffer(temp);
                object* objPtr = SearchobjByID(tempPtr);
                delete[] tempPtr;

                fin >> ws;
                fin.getline(temp, 300);

                postPtr->AddComment(commentPtr);
                commentPtr->SetCommentValues(commentID, temp, objPtr);
                totalComments++;
            }

            fin.close();
        }
        else
        {
            cout << "< Cannot Open Comments Input File >";
            throw exception();
        }
    }
    void LoadAllActivities(const char* fileName)
    {
        ifstream fin(fileName);
        if (fin.is_open())
        {
            fin >> totalActivities;


            char tempID[30];
            char* tempPtr;
            for (int i = 0; i < totalActivities; i++)
            {
                PostContent* actPtr = new Activity;

                fin >> tempID;
                tempPtr = Helper::GetStringFromBuffer(tempID);
                Post* postPtr = SearchPostByID(tempPtr);
                delete[] tempPtr;
                if (postPtr == 0)
                    continue;

                actPtr->ReadDataFromFile(fin);
                postPtr->AddContent(actPtr);
            }

            fin.close();
        }
        else
        {
            cout << "< Cannot Open Comments Input File >";
            throw exception();
        }
    }
    void LoadData()
    {
        LoadAllUsers("SocialNetworkUsers.txt");
        LoadAllPages("SocialNetworkPages.txt");
        LinkUsersAndPages("UsersFriendsAndLikedPages.txt");
        LoadAllPosts("SocialNetworkPosts.txt");
        LoadAllComments("SocialNetworkComments.txt");
        LoadAllActivities("SocialNetworkActivities.txt");
    }

    void Run()
    {
        int day = 17, month = 4, year = 2024;
        cout << "_________________________________________________________________________________________" << endl;
        cout << "\t\tCommand :\t\t Set current System Date" << endl;
        cout << "_________________________________________________________________________________________" << endl;
        Date::setCurrentdate(day, month, year);

        cout << "_________________________________________________________________________________________" << endl;
        cout << "\t\tCommand :\t\t Set current User" << endl;
        cout << "_________________________________________________________________________________________" << endl;
        User* currentUser = SearchUserByID("u7");
        if (currentUser == NULL)
        {
            cout << "< User Id Not Found >" << endl;
            return;
        }
        else
        {
            currentUser->printName();
            cout << " successfully set as Current User :)\n" << endl;
        }

        cout << "_________________________________________________________________________________________" << endl;
        cout << "\t\tCommand :\t\t View Friend List" << endl;
        cout << "_________________________________________________________________________________________" << endl;
        currentUser->ViewFriendList();
        cout << "\n\n_________________________________________________________________________________________" << endl;
        cout << "\t\tCommand :\t\t View Liked Pages" << endl;
        cout << "_________________________________________________________________________________________" << endl;
        currentUser->ViewLikedPages();
        cout << "\n\n_________________________________________________________________________________________" << endl;
        cout << "\t\tCommand :\t\t View TimeLine" << endl;
        cout << "_________________________________________________________________________________________" << endl;
        currentUser->ViewTimeline();

        cout << "_________________________________________________________________________________________" << endl;
        cout << "\t\tCommand :\t\t View Liked List (post5)" << endl;
        cout << "_________________________________________________________________________________________" << endl;
        Post* post = SearchPostByID("post5");
        if (post == NULL)
        {
            cout << "< Post Not Found >" << endl;
            return;
        }
        post->ViewLikedList();

        object* currentobj = currentUser;
        cout << "\n_________________________________________________________________________________________" << endl;
        cout << "\t\tCommand :\t\t Like Post (post5)" << endl;
        cout << "_________________________________________________________________________________________" << endl;
        post->setlikedBy(currentobj);
        cout << "\n" << post->GetID() << " - 1 new like by ";
        currentobj->printName();

        cout << "\n\n_________________________________________________________________________________________" << endl;
        cout << "\t\tCommand :\t\t View Liked List (post5)" << endl;
        cout << "_________________________________________________________________________________________" << endl;
        post->ViewLikedList();

        cout << "\n\n_________________________________________________________________________________________" << endl;
        cout << "\t\tCommand :\t\t View Page (p1)" << endl;
        cout << "_________________________________________________________________________________________" << endl;
        currentobj = SearchPageByID("p1");
        if (currentobj == NULL)
        {
            cout << "< Page Id Not Found >" << endl;
            return;
        }
        currentobj->ViewTimeline();

        cout << "\n_________________________________________________________________________________________" << endl;
        cout << "\t\tCommand :\t\t View Home" << endl;
        cout << "_________________________________________________________________________________________" << endl;
        currentUser->ViewHome();

        cout << "\n__________________________________________________________________________________________" << endl;
        cout << "\t\tCommand :\t\t  Post Comment(post4, Good Luck for your Result)" << endl;
        cout << "__________________________________________________________________________________________" << endl;
        post = SearchPostByID("post4");
        if (post == NULL)
        {
            cout << "< Post Not Found >" << endl;
            return;
        }
        comment* newComment = new comment;
        char* id = Helper::IdGenerator("c", totalComments);
        currentobj = currentUser;
        newComment->SetCommentValues(id, "Good Luck for your Result", currentobj);
        delete[] id;
        post->AddComment(newComment);
        cout << "\n" << post->GetID() << " - 1 new Comment by ";
        currentobj->printName();

        cout << "\n__________________________________________________________________________________________" << endl;
        cout << "\t\tCommand :\t\t  View Post(post4)" << endl;
        cout << "__________________________________________________________________________________________" << endl;
        post->PrintPostDetails();

        cout << "\n__________________________________________________________________________________________" << endl;
        cout << "\t\tCommand :\t\t   PostComment(post8, Thanks for the wishes)" << endl;
        cout << "__________________________________________________________________________________________" << endl;
        post = SearchPostByID("post8");
        if (post == NULL)
        {
            cout << "< Post Not Found >" << endl;
            return;
        }
        newComment = new comment;
        id = Helper::IdGenerator("c", totalComments);
        currentobj = currentUser;
        newComment->SetCommentValues("c15", "Thanks for the wishes", currentobj);
        delete[] id;
        post->AddComment(newComment);
        cout << "\n" << post->GetID() << " - 1 new Comment by ";
        currentobj->printName();

        cout << "\n__________________________________________________________________________________________" << endl;
        cout << "\t\tCommand :\t\t  View Post(post8)" << endl;
        cout << "__________________________________________________________________________________________" << endl;
        post->PrintPostDetails();

        cout << "\n__________________________________________________________________________________________" << endl;
        cout << "\t\tCommand :\t\t  See Your Memories" << endl;
        cout << "__________________________________________________________________________________________" << endl;
        currentUser->ViewMemory();

        cout << "\n______________________________________________________________________________________________" << endl;
        cout << "\tCommand :\tShareMemory(post10,Never thought I will be specialist in this field)" << endl;
        cout << "______________________________________________________________________________________________" << endl;
        post = SearchPostByID("post10");
        id = Helper::IdGenerator("post", totalPosts);
        Post* newMemory = new memory("Never thought I will be specialist in this field...", currentobj, post, "post13", Date::currentDate);
        delete[] id;
        currentUser->AddToTimeline(newMemory);
        currentUser->printName();
        cout << " shared a memory" << endl;

        cout << "\n__________________________________________________________________________________________" << endl;
        cout << "\t\tCommand :\t\t  View TimeLine" << endl;
        cout << "__________________________________________________________________________________________" << endl;
        currentUser->ViewTimeline();

        cout << "_________________________________________________________________________________________" << endl;
        cout << "\t\tCommand :\t\t Set current User" << endl;
        cout << "_________________________________________________________________________________________" << endl;
        currentUser = SearchUserByID("u11");
        if (currentUser == NULL)
        {
            cout << "< User Id Not Found >" << endl;
            return;
        }
        else
        {
            currentUser->printName();
            cout << " successfully set as Current User :)\n" << endl;
        }

        cout << "\n__________________________________________________________________________________________" << endl;
        cout << "\t\tCommand :\t\t  View TimeLine" << endl;
        cout << "__________________________________________________________________________________________" << endl;
        currentUser->ViewTimeline();
    }

};

int Controller::totalUsers = 0;
int Controller::totalPages = 0;
int Controller::totalPosts = 0;
int Controller::totalComments = 0;
int Controller::totalActivities = 0;

int main()
{
    Controller mainApp;
    mainApp.LoadData();
    mainApp.Run();

    system("pause");

    return 0;
}