# ConnectGram

A console-based social media platform implemented in **C++** as a Data Structures course project. The application demonstrates the practical use of fundamental data structures by implementing core social networking features such as user management, posting, searching, trending hashtags, personalized feeds, and friend recommendations.

## Features

- User authentication
  - Sign up
  - Login

- Social interactions
  - Follow users
  - Unfollow users
  - Like posts

- Posting system
  - Create text posts
  - Hashtag support
  - Personalized feed

- Search
  - Username autocomplete using a Trie
  - Smart user search
  - Search history

- Trending
  - Trending hashtags
  - Popular post discovery

- Friend recommendation
  - Suggest friends using graph traversal (BFS)

- Persistent storage
  - Save and load users
  - Save and load posts
  - Save and load hashtags
  - JSON-based storage

---

## Data Structures Used

This project was designed to demonstrate several fundamental data structures:

| Data Structure | Purpose |
|---------------|---------|
| Hash Map | Fast lookup for users, posts, and hashtags |
| Hash Set | Store hashtag-post relationships |
| Trie | Username prefix search and autocomplete |
| Max Heap | Ranking trending hashtags/posts |
| Queue | Search history management |
| Graph (BFS) | Friend recommendation |
| Dynamic Arrays / STL Containers | General data management |

---

## Project Structure

```
ConnectGram/
│
├── main.cpp
├── app_singleton.*
├── user.*
├── post.*
├── trie.*
├── hash_map.*
├── hash_set.*
├── max_heap.*
├── manage_file_storage.*
├── json.hpp
└── ...
```

---

## Supported Commands

| Command | Description |
|---------|-------------|
| `signup <username> <password>` | Create a new account |
| `login <username> <password>` | Login |
| `search_user <prefix>` | Search users by username prefix |
| `follow <username>` | Follow a user |
| `unfollow <username>` | Unfollow a user |
| `post "<content>"` | Create a new post |
| `trend <#hashtag>` | View hashtag trend |
| `like <post_id>` | Like a post |
| `show_feed` | Display personalized feed |
| `smart_search <username>` | Intelligent user search |
| `show_history` | Show recent searches |
| `suggest_friends` | Recommend friends |
| `exit` | Save data and exit |

---

## Technologies

- C++17
- Object-Oriented Programming (OOP)
- Custom Data Structures
- JSON Serialization
- File I/O

---

## Learning Objectives

This project demonstrates practical implementations of:

- Hash Tables
- Trie
- Heap
- Graph Traversal (BFS)
- Queues
- Object-Oriented Design
- File Management
- Data Persistence

---

## Build

Compile using any C++17 compatible compiler.


## License

This project was developed for educational purposes as part of a Data Structures course.
