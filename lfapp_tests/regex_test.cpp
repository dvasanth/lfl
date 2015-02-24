/*
 * $Id$
 * Copyright (C) 2009 Lucid Fusion Labs

 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "gtest/gtest.h"
#include "lfapp/lfapp.h"

GTEST_API_ int main(int argc, const char **argv) {
    testing::InitGoogleTest(&argc, (char**)argv);
    LFL::FLAGS_default_font = LFL::FakeFont::Filename();
    CHECK_EQ(LFL::app->Create(argc, argv, __FILE__), 0);
    return RUN_ALL_TESTS();
}

namespace LFL {
DEFINE_int(size, 1024*1024, "Test size"); 

struct MyEnvironment : public ::testing::Environment {
    string test1 = "yeah fun http://url and whatever ";
    virtual ~MyEnvironment() {}
    virtual void TearDown() { INFO(Singleton<PerformanceTimers>::Get()->DebugString()); }
    virtual void SetUp() {}
};

MyEnvironment* const my_env = (MyEnvironment*)::testing::AddGlobalTestEnvironment(new MyEnvironment);

template <class K> struct TrieNode {
    struct Next { K key; int next; };
    int val=0, next_ind=0;
    unsigned short next_len=0, next_i=0;
    TrieNode(int V=0) : val(V) {}
};

template <class K, class V, class TN = TrieNode<K> > struct Trie {
    typedef Trie<K, V, TN> Self;
    typedef basic_string<K> String;

    struct SortedInputConstructor {
        struct Node {
            K key; int node, next_len; 
            Node(const K &k, int n) : key(k), node(n), next_len(0) {}
        };
        typedef void (SortedInputConstructor::*InsertCB)  (const V*);
        typedef void (SortedInputConstructor::*CompleteCB)(const Node&);

        Self *out;
        int insert_id=0;
        bool patricia=true;
        vector<Node> stack;
        InsertCB     insert_cb = &SortedInputConstructor::Phase1Insert;
        CompleteCB complete_cb = &SortedInputConstructor::Phase1Complete;

        SortedInputConstructor(Self *O, const vector<String> &in, bool P=true) : out(O), patricia(P) {
            stack.emplace_back(K(), insert_cb(NULL));
            Input(in);
            insert_cb   = &SortedInputConstructor::Phase2Insert;
            complete_cb = &SortedInputConstructor::Phase2Complete;
            Input(in); 
        }

        void Input(const vector<String> &in) { 
            for (auto i = in.begin(); i != in.end(); ++i) Input(*i, i != in.end() ? *(i+1) : "", V());
            while (stack.size()) complete_cb(PopBack(stack));
            out->head = 1;
        }
        void Input(const String &in, const String &lookahead, const V &v) {
            PopPrefixesNotMatching(in);
            AddPrefixesOf(in, lookahead);
            stack.emplace_back(in[stack.size()-1], insert_cb(&v));
        }

        int Phase1Insert(const V *v) { out->data.push_back(TN(v ? out->val.push_back(v)+1 : 0)+1); }
        void Phase1Complete(const Node &n) {
            if (stack.size()) stack.back().next_len++;
            out->data[n.node-1].next_len = n.next_len;
        }

        int Phase2Insert(const V *v) { return insert_id++; }
        void Phase2Complete(const Node &n) {
            if (!stack.size()) return;
            TN *p = out->data[stack.back().node-1];
            CHECK_LT(p->next_i, p->next_len);
            if (!p->next_ind) {
                p->next_ind = out->next.size() + 1;
                out->next.resize(out->next.size() + p->next_len);
            }
            int *next = &out->next[p->next_ind-1];
            next[p->next_i++] = TN::Next(n.key, n.node);
        }

        void PopPrefixesNotMatching(const String &in) {
            auto si = stack.begin() + 1, se = stack.end();
            for (auto i = in.begin(), e = in.end(); i != e && si != se; ++i, ++si) if (*i != e->key) break;
            for (int pop = se - si; pop > 0; pop--) complete_cb(PopBack(stack));
        }
        void AddPrefixesOf(const String &in, const String &lookahead) {
            int add_prefixes;
            if (patricia) {
                int matching_lookahead = MismatchOffset(in.begin(), in.end(), lookahead.begin(), lookahead.end()); 
                add_prefixes = matching_lookahead - stack.size() - (matching_lookahead == in.size());
            } else add_prefixes = in.size() - stack.size() + 1;
            for (int i=0; i<add_prefixes; i++) stack.emplace_back(in[stack.size()-1], insert_cb(TN())+1);
        }
    };

    vector<TrieNode<K> > data;
    vector<typename TrieNode<K>::Next> next;
    vector<V> val;
    int head=0;
    Trie(const vector<String> &sorted_input) { SortedInputConstructor(this); }
};

struct AhoCorasickMatcher {
    unsigned fail;
    AhoCorasickMatcher(const vector<string> &sorted_in) {
    }

};

TEST(RegexTest, StrstrURL) {
    PerformanceTimers *timers = Singleton<PerformanceTimers>::Get();
    int tid = timers->Create("StrstrURL");
    timers->AccumulateTo(tid);
    for (int i=0; i<FLAGS_size; ++i) {
        const char *match = strstr(my_env->test1.c_str(), "http://");
        EXPECT_NE(nullptr, match);
        EXPECT_EQ(0, strncmp(match, "http://", 7));
    }
    timers->AccumulateTo(0);
}

TEST(RegexTest, RegexpURL) {
    PerformanceTimers *timers = Singleton<PerformanceTimers>::Get();
    int tid = timers->Create("RegexpURL");

    vector<Regex::Result> matches;
    Regex url_matcher("https?://");
    timers->AccumulateTo(tid);
    for (int i=0; i<FLAGS_size; ++i) {
        url_matcher.Match(my_env->test1, &matches);
        EXPECT_EQ(1, matches.size()); EXPECT_EQ("http://", matches[0].Text(my_env->test1));
        matches.clear();
    }
    timers->AccumulateTo(0);
}

TEST(RegexTest, RegexStreamURL) {
    PerformanceTimers *timers = Singleton<PerformanceTimers>::Get();
    int tid = timers->Create("StreamURL");

    vector<Regex::Result> matches;
    StreamRegex url_matcher("https?://");
    timers->AccumulateTo(tid);
    for (int i=0; i<FLAGS_size; ++i) {
        url_matcher.Match(my_env->test1, &matches);
        EXPECT_EQ(1, matches.size()); EXPECT_EQ("http://", matches[0].Text(my_env->test1));
        matches.clear();
    }
    timers->AccumulateTo(0);
}

}; // namespace LFL
