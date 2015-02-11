/*
 * $Id: lftypes.h 1335 2014-12-02 04:13:46Z justin $
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

#ifndef __LFL_LFAPP_LFTYPES_H__
#define __LFL_LFAPP_LFTYPES_H__

#ifdef LFL_JUDY
#include "judymap.h"
#endif

#ifdef WIN32
#include <intrin.h>
#pragma intrinsic(_BitScanForward)
static __forceinline int ffsl(long x) {
	unsigned long i;
	if (_BitScanForward(&i, x)) return (i + 1);
	return (0);
}
static __forceinline int ffs(int x) { return ffsl(x); }
#endif

#define SortImpl1(x1, y2) return x1 < y2;
#define SortImpl2(x1, y1, x2, y2)  \
    if      (x1 < y1) return true;  \
    else if (x1 > y1) return false; \
    else return x2 < y2;
#define SortImpl3(x1, y1, x2, y2, x3, y3) \
    if      (x1 < y1) return true;  \
    else if (x1 > y1) return false; \
    if      (x2 < y2) return true;  \
    else if (x2 > y2) return false; \
    else return x3 < y3;
#define SortImpl4(x1, y1, x2, y2, x3, y3, x4, y4) \
    if      (x1 < y1) return true;  \
    else if (x1 > y1) return false; \
    if      (x2 < y2) return true;  \
    else if (x2 > y2) return false; \
    if      (x3 < y3) return true;  \
    else if (x3 > y3) return false; \
    else return x4 < y4;

namespace LFL {
template <typename K, typename V> typename map<K, V>::iterator FindOrInsert(map<K, V> &m, K k, V v, int *inserted) {
    LFL_STL_NAMESPACE::pair<typename map<K, V>::iterator, bool> ret = m.insert(typename map<K, V>::value_type(k, v));
    if (inserted) (*inserted)++;
    return ret.first;
}

template <typename K, typename V> typename map<K, V>::iterator FindOrInsert(map<K, V> &m, K k, V v, bool *inserted=0) {
    LFL_STL_NAMESPACE::pair<typename map<K, V>::iterator, bool> ret = m.insert(typename map<K, V>::value_type(k, v));
    if (inserted) *inserted = ret.second;
    return ret.first;
}

template <typename K, typename V> typename map<K, V*>::iterator FindOrInsert(map<K, V*> &m, K k, V* v, bool *inserted=0) {
    LFL_STL_NAMESPACE::pair<typename map<K, V*>::iterator, bool> ret = m.insert(typename map<K, V*>::value_type(k, v));
    if (inserted) *inserted = ret.second;
    return ret.first;
}

template <typename K, typename V> typename map<K, V*>::iterator FindOrInsert(map<K, V*> &m, K k, bool *inserted=0) {
    LFL_STL_NAMESPACE::pair<typename map<K, V*>::iterator, bool> ret = m.insert(typename map<K, V*>::value_type(k, 0));
    if (ret.second) ret.first->second = new V();
    if (inserted) *inserted = ret.second;
    return ret.first;
}

template <typename K, typename V> typename unordered_map<K, V>::iterator FindOrInsert(unordered_map<K, V> &m, K k, V v, bool *inserted) {
    LFL_STL_NAMESPACE::pair<typename unordered_map<K, V>::iterator, bool> ret = m.insert(typename unordered_map<K, V>::value_type(k, v));
    if (inserted) *inserted = ret.second;
    return ret.first;
}

template <typename X> typename X::mapped_type FindOrNull(const X &m, const typename X::key_type &k) {
    typename X::const_iterator iter = m.find(k);
    return (iter != m.end()) ? iter->second : 0;
}

template <typename X> typename X::mapped_type FindOrDefault(const X &m, const typename X::key_type &k, const typename X::mapped_type &v) {
    typename X::const_iterator iter = m.find(k);
    return (iter != m.end()) ? iter->second : v;
}

template <typename X> typename X::mapped_type FindOrDie(const char *file, int line, const X &m, const typename X::key_type &k) {
    typename X::const_iterator iter = m.find(k);
    if (iter == m.end()) Log(LFApp::Log::Fatal, file, line, StrCat("FindOrDie(", k, ")"));
    return iter->second;
}
#define FindOrDie(m, k) FindOrDie(__FILE__, __LINE__, m, k)

template <typename X> bool Contains(const X &c, const typename X::key_type &k) { return c.find(k) != c.end(); }
template <typename X> void EnsureSize(X &x, int n) { if (x.size() < n) x.resize(n); }

template <typename X> typename X::value_type &PushFront(X &v, const typename X::value_type &x) { v.push_front(x); return v.front(); }
template <typename X> typename X::value_type &PushBack (X &v, const typename X::value_type &x) { v.push_back (x); return v.back (); }
template <typename X> typename X::value_type  PopFront (X &v) { typename X::value_type ret = v.front(); v.pop_front(); return ret; }
template <typename X> typename X::value_type  PopBack  (X &v) { typename X::value_type ret = v.back (); v.pop_back (); return ret; }

template <class X>       X *VectorGet(      vector<X> &x, int n) { return (n >= 0 && n < x.size()) ? &x[n] : 0; }
template <class X> const X *VectorGet(const vector<X> &x, int n) { return (n >= 0 && n < x.size()) ? &x[n] : 0; }
template <class X> X *VectorEnsureElement(vector<X> &x, int n) { EnsureSize(x, n+1); return &x[n]; }
template <class X> X *VectorCheckElement(vector<X> &x, int n) { CHECK_LT(n, x.size()); return &x[n]; }
template <typename X, class Y> void VectorAppend(vector<X> &out, const Y& begin, const Y& end) {
    out.insert(out.end(), begin, end);
}

template <class X> void VectorClear(vector<X > *v) { v->clear(); }
template <class X> void VectorClear(vector<X*> *v) {
    for (typename vector<X*>::const_iterator i = v->begin(); i != v->end(); ++i) delete (*i);
    v->clear();
}
template <class X> void VectorErase(X* v, int ind) {
    CHECK_RANGE(ind, 0, v->size());
    v->erase(v->begin()+ind, v->begin()+ind+1);
}
template <class X> int VectorEraseByValue(vector<X> *v, const X& x) {
    int orig_size = v->size();
    v->erase(LFL_STL_NAMESPACE::remove(v->begin(), v->end(), x), v->end());
    return orig_size - v->size();
}

template <class X> X BackOrDefault (const vector<X> &a)                    { return a.size() ? a.back () : X(); }
template <class X> X FrontOrDefault(const vector<X> &a)                    { return a.size() ? a.front() : X(); }
template <class X> X IndexOrDefault(const vector<X> &a, int n)             { return n < a. size() ?   a [n] : X(); }
template <class X> X IndexOrDefault(      vector<X> *a, int n)             { return n < a->size() ? (*a)[n] : X(); }
template <class X> X IndexOrDefault(const vector<X> &a, int n, const X& b) { return n < a. size() ?   a [n] : b; }

template <class X> void InsertOrErase(X *v, const typename X::value_type &val, bool insert) {
    if (insert) v->insert(val);
    else        v->erase(val);
}

template <class X> void FilterValues(X *v, const typename X::value_type &val) {
    for (typename X::iterator i = v->begin(); i != v->end(); /**/) {
        if (*i == val) i = v->erase(i);
        else i++;
    }
}

template <class X> void Move(X *to, const X *from, int size) {
    if      (from == to) return;
    else if (from <  to) { for (int i=size-1; i >= 0; i--) to[i] = from[i]; }
    else                 { for (int i=0; i <= size-1; i++) to[i] = from[i]; }
} 

template <class X> void Move(X &buf, int to_ind, int from_ind, int size) {
    if      (from_ind == to_ind) return;
    else if (from_ind <  to_ind) { for (int i=size-1; i >= 0; i--) buf[to_ind+i] = buf[from_ind+i]; }
    else                         { for (int i=0; i <= size-1; i++) buf[to_ind+i] = buf[from_ind+i]; }
} 

template <class X, class Y> void Move(X &buf, int to_ind, int from_ind, int size, const function<void(Y&,Y&)> &move_cb) {
    if      (from_ind == to_ind) return;
    else if (from_ind <  to_ind) { for (int i=size-1; i >= 0; i--) move_cb(buf[to_ind+i], buf[from_ind+i]); }
    else                         { for (int i=0; i <= size-1; i++) move_cb(buf[to_ind+i], buf[from_ind+i]); }
} 

template <class X> struct ScopedValue {
    X *v, ov;
    ScopedValue(X *V, X nv) : v(V), ov(V ? *V : X()) { *v = nv; }
    ~ScopedValue() { if (v) *v = ov; }
};

template <class X> struct ScopedDeltaTracker {
    X *v, ov; function<X()> f;
    ScopedDeltaTracker(X *V, const function<X()> &F) : v(V), ov(V ? F() : X()), f(F) {}
    ~ScopedDeltaTracker() { if (v) (*v) += (f() - ov); }
};

template <class T1, class T2, class T3> struct Triple {
    T1 first; T2 second; T3 third;
    Triple() {}
    Triple(const T1 &t1, const T2 &t2, const T3 &t3) : first(t1), second(t2), third(t3) {}
    Triple(const Triple &copy) : first(copy.first), second(copy.second), third(copy.third) {}
    const Triple &operator=(const Triple &r) { first=r.first; second=r.second; third=r.third; return *this; }
    bool operator<(const Triple &r) const { SortImpl3(first, r.first, second, r.second, third, r.third); }
};

template <class T1, class T2, class T3, class T4> struct Quadruple {
    T1 first; T2 second; T3 third; T4 fourth;
    Quadruple() {}
    Quadruple(const T1 &t1, const T2 &t2, const T3 &t3, const T4 &t4) : first(t1), second(t2), third(t3), fourth(t4) {}
    Quadruple(const Quadruple &copy) : first(copy.first), second(copy.second), third(copy.third), fourth(copy.fourth) {}
    const Quadruple &operator=(const Quadruple &r) { first=r.first; second=r.second; third=r.third; fourth=r.fourth; return *this; }
    bool operator<(const Quadruple &r) const { SortImpl4(first, r.first, second, r.second, third, r.third, fourth, r.fourth); }
};

template <class X> struct FreeListVector {
    vector<X> data;
    vector<int> free_list;

    int size() const { return data.size(); }
    const X& back() const { return data.back(); }
    const X& operator[](int i) const { return data[i]; }
    /**/  X& operator[](int i)       { return data[i]; }

    virtual void Clear() { data.clear(); free_list.clear(); }
    virtual int Insert(const X &v) {
        if (free_list.empty()) {
            data.push_back(v);
            return data.size()-1;
        } else {
            int free_ind = PopBack(free_list);
            data[free_ind] = v;
            return free_ind;
        }
    }
    virtual void Erase(unsigned ind) {
        CHECK_LT(ind, data.size());
        free_list.push_back(ind);
    }
};

template <class X> struct IterableFreeListVector : public FreeListVector<X> { /// Expects X.deleted
    virtual void Erase(unsigned ind) {
        FreeListVector<X>::Erase(ind);
        FreeListVector<X>::data[ind].deleted = 1;
    }
};

template <typename X> struct ArraySegmentIter {
    const X *buf; int i, ind, len, cur_start; X cur_attr;
    ArraySegmentIter(const basic_string<X> &B) : buf(B.size() ? &B[0] : 0), i(-1), ind(0), len(B.size()) { Increment(); }
    ArraySegmentIter(const vector      <X> &B) : buf(B.size() ? &B[0] : 0), i(-1), ind(0), len(B.size()) { Increment(); }
    ArraySegmentIter(const X *B, int L)        : buf(B),                    i(-1), ind(0), len(L)        { Increment(); }
    const X *Data() const { return &buf[cur_start]; }
    int Length() const { return ind - cur_start; }
    bool Done() const { return cur_start == len; }
    void Update() { cur_start = ind; if (!Done()) cur_attr = buf[ind]; }
    void Increment() { Update(); while (ind != len && cur_attr == buf[ind]) ind++; i++; }
};

template <typename X, typename Y, Y (X::*Z)> struct ArrayMemberSegmentIter {
    typedef function<void (const X&)> CB;
    typedef function<bool (const Y&, const Y&)> Cmp;
    const X *buf; int i, ind, len, cur_start; Y cur_attr; CB cb; Cmp cmp;
    ArrayMemberSegmentIter(const vector<X> &B)              : buf(B.size() ? &B[0] : 0), i(-1), ind(0), len(B.size()),         cmp(equal_to<Y>()) { Increment(); }
    ArrayMemberSegmentIter(const X *B, int L)               : buf(B),                    i(-1), ind(0), len(L),                cmp(equal_to<Y>()) { Increment(); }
    ArrayMemberSegmentIter(const X *B, int L, const CB &Cb) : buf(B),                    i(-1), ind(0), len(L),        cb(Cb), cmp(equal_to<Y>()) { Increment(); }
    const X *Data() const { return &buf[cur_start]; }
    int Length() const { return ind - cur_start; }
    bool Done() const { return cur_start == len; }
    void Update() { cur_start = ind; if (!Done()) cur_attr = buf[ind].*Z; }
    void Increment() { Update(); while (ind != len && cmp(cur_attr, buf[ind].*Z)) { if (cb) cb(buf[ind]); ind++; } i++; }
};

template <typename X, typename Y, Y (X::*Z1), Y (X::*Z2)> struct ArrayMemberPairSegmentIter {
    typedef function<void (const X&)> CB;
    typedef function<bool (const Y&, const Y&)> Cmp;
    const X *buf; int i, ind, len, cur_start; Y cur_attr1, cur_attr2; CB cb; Cmp cmp;
    ArrayMemberPairSegmentIter(const vector<X> &B)              : buf(B.size() ? &B[0] : 0), i(-1), ind(0), len(B.size()),         cmp(equal_to<Y>()) { Increment(); }
    ArrayMemberPairSegmentIter(const X *B, int L)               : buf(B),                    i(-1), ind(0), len(L),                cmp(equal_to<Y>()) { Increment(); }
    ArrayMemberPairSegmentIter(const X *B, int L, const CB &Cb) : buf(B),                    i(-1), ind(0), len(L),        cb(Cb), cmp(equal_to<Y>()) { Increment(); }
    const X *Data() const { return &buf[cur_start]; }
    int Length() const { return ind - cur_start; }
    bool Done() const { return cur_start == len; }
    void Update() { cur_start = ind; if (!Done()) { const X& b = buf[ind]; cur_attr1 = b.*Z1; cur_attr2 = b.*Z2; } }
    void Increment() { Update(); while (ind != len) { const X& b = buf[ind]; if (!cmp(cur_attr1, b.*Z1) || !cmp(cur_attr2, b.*Z2)) break; if (cb) cb(buf[ind]); ind++; } i++; }
};

template <typename X, typename Y, Y (X::*Z)() const> struct ArrayMethodSegmentIter {
    typedef function<void (const X&)> CB;
    typedef function<bool (const Y&, const Y&)> Cmp;
    const X *buf; int i, ind, len, cur_start; Y cur_attr; CB cb; Cmp cmp;
    ArrayMethodSegmentIter(const vector<X> &B)              : buf(B.size() ? &B[0] : 0), i(-1), ind(0), len(B.size()),         cmp(equal_to<Y>()) { Increment(); }
    ArrayMethodSegmentIter(const X *B, int L)               : buf(B),                    i(-1), ind(0), len(L),                cmp(equal_to<Y>()) { Increment(); }
    ArrayMethodSegmentIter(const X *B, int L, const CB &Cb) : buf(B),                    i(-1), ind(0), len(L),        cb(Cb), cmp(equal_to<Y>()) { Increment(); }
    const X *Data() const { return &buf[cur_start]; }
    int Length() const { return ind - cur_start; }
    bool Done() const { return cur_start == len; }
    void Update() { cur_start = ind; if (!Done()) cur_attr = (buf[ind].*Z)(); }
    void Increment() { Update(); while (ind != len && cmp(cur_attr, (buf[ind].*Z)())) { if (cb) cb(buf[ind]); ind++; } i++; }
};

template <class X> struct FlattenedArrayValues {
    typedef pair<int, int> Iter;
    typedef function<int(X*, int)> GetValCB;
    X *data; int l; GetValCB get_val;
    FlattenedArrayValues(X *D, int L, GetValCB cb) : data(D), l(L), get_val(cb) {}

    Iter LastIter() { return Iter(X_or_1(l)-1, l ? get_val(data, l-1)-1 : 0); }
    void AdvanceIter(Iter *o, int n) {
        if (!l) return;
        for (int i=abs(n), d; i; i-=d) {
            int v = get_val(data, o->first);
            if (n > 0) {
                d = min(i, v - o->second);
                if ((o->second += d) >= v) {
                    if (o->first >= l-1) { *o = LastIter(); return; }
                    *o = Iter(o->first+1, 0);
                }
            } else {
                d = min(i, o->second+1);
                if ((o->second -= d) < 0) {
                    if (o->first <= 0) { *o = Iter(0, 0); return; }
                    *o = Iter(o->first-1, get_val(data, o->first-1)-1);
                }
            }
        }
    }
    int Distance(Iter i1, Iter i2, int maxdist=0) {
        int dist = 0;
        if (i2 < i1) swap(i1, i2);
        for (int i = i1.first; i <= i2.first; i++) {
            int v = get_val(data, i);
            dist += v;
            if (i == i1.first) dist -= i1.second;
            if (i == i2.first) dist -= (v - i2.second);
            if (maxdist && dist >= maxdist) return maxdist;
        }
        return dist;
    }
};

template <class X> struct ValueSet {
    int i, c; X *d;
    int Match(X x) { for (int j=0; j<c; j++) if (x == d[j]) return j; return -1; }
    ValueSet(int n, ...) : i(0), c(n), d(new X[c]) { va_list ap; va_start(ap, n); for (int j=0; j<c; j++) d[j] = va_arg(ap, X); va_end(ap); }
    ~ValueSet() { delete[] d; }
    X Cur() { return d[i]; }
    X Next() { i=(i+1)%c; return d[i]; }
    bool Enabled() { return i!=0; }
    void On() { i=1; }
    void Off(){ i=0; }
};

struct AnyBoolSet {
    int count;
    AnyBoolSet() : count(0) {}
    bool Get() const { return count; }
};

struct AnyBoolElement {
    AnyBoolSet *parent; bool val;
    AnyBoolElement(AnyBoolSet *P, bool v=0) : parent(P), val(0) { Set(v); }
    virtual ~AnyBoolElement() { Set(0); }
    bool Get() const { return val; }
    void Set(bool v) {
        if (val == v) return;
        if (v) parent->count++;
        else   parent->count--;
        val = v;
    }
};

struct ToggleBool {
    enum { Default = 1, OneShot = 2 };
    bool *toggle; int mode;
    ToggleBool(bool *t, int m=Default) : toggle(t), mode(m) {}
    bool Toggle() { if (*toggle && mode == OneShot) return false; *toggle = !*toggle; return true; }
};

struct ReallocHeap {
    char *heap; int size, len;
    ~ReallocHeap(); 
    ReallocHeap(int startSize=65536);
    void Reset() { len=0; }
    int Alloc(int len);
};

struct Bit {
    static int Count(unsigned long long n) { int c; for (c=0; n; c++) n &= (n-1); return c; }
    static void Indices(unsigned long long in, int *o) { 
        for (int b, n = in & 0xffffffff; n; /**/) { b = ffs(n)-1; n ^= (1<<b); *o++ = b;    }
        for (int b, n = in >> 32       ; n; /**/) { b = ffs(n)-1; n ^= (1<<b); *o++ = b+32; }
        *o++ = -1;
    }
};

struct BitField {
    static int Clear(      unsigned char *b, int bucket) {          b[bucket/8] &= ~(1 << (bucket % 8)); return 1; }
    static int Set  (      unsigned char *b, int bucket) {          b[bucket/8] |=  (1 << (bucket % 8)); return 1; }
    static int Get  (const unsigned char *b, int bucket) { return   b[bucket/8] &   (1 << (bucket % 8));           }
    static int Not  (const unsigned char *b, int bucket) { return !(b[bucket/8] &   (1 << (bucket % 8)));          }
    static int Get  (      unsigned char *b, int bucket) { return Get((const unsigned char*)b, bucket); }
    static int Not  (      unsigned char *b, int bucket) { return Not((const unsigned char*)b, bucket); }

    static int FirstSet  (const unsigned char *b, int l) { for (int i=0;   i<l;  i++) { unsigned char c=b[i]; if (c != 0)   return i*8 + ffs( c)-1; } return -1; }
    static int FirstClear(const unsigned char *b, int l) { for (int i=0;   i<l;  i++) { unsigned char c=b[i]; if (c != 255) return i*8 + ffs(~c)-1; } return -1; }
    static int  LastClear(const unsigned char *b, int l) { for (int i=l-1; i>=0; i--) { unsigned char c=b[i]; if (c != 255) return i*8 + ffs(~c)-1; } return -1; }
};

struct BloomFilter {
    int M, K; unsigned char *buf;
    ~BloomFilter() { free(buf); }
    BloomFilter(int m, int k, unsigned char *B) : M(m), K(k), buf(B) {}
    static BloomFilter *Empty(int M, int K) { int s=M/8+1; BloomFilter *bf = new BloomFilter(M, K, (unsigned char*)calloc(s, 1)); return bf; }
    static BloomFilter *Full (int M, int K) { int s=M/8+1; BloomFilter *bf = new BloomFilter(M, K, (unsigned char*)malloc(s)); memset(bf->buf, ~0, s); return bf; }

    void Set  (long long val) { return Set  ((      unsigned char *)&val, sizeof(long long)); }
    void Clear(long long val) { return Clear((      unsigned char *)&val, sizeof(long long)); }
    int  Get  (long long val) { return Get  ((const unsigned char *)&val, sizeof(long long)); }
    int  Not  (long long val) { return Not  ((const unsigned char *)&val, sizeof(long long)); }

    void Set  (      unsigned char *val, size_t len) {        ForEachBitBucketDo(BitField::Set,   val, len); }
    void Clear(      unsigned char *val, size_t len) {        ForEachBitBucketDo(BitField::Clear, val, len); }
    int  Get  (const unsigned char *val, size_t len) { return ForEachBitBucketDo(BitField::Get,   val, len); }
    int  Not  (const unsigned char *val, size_t len) { return ForEachBitBucketDo(BitField::Not,   val, len); }

    int ForEachBitBucketDo(int (*op)(unsigned char *b, int bucket), const unsigned char *val, size_t len) {
        unsigned long long h = fnv64(val, len);
        unsigned h1 = (h >> 32) % M, h2 = (h & 0xffffffff) % M;
        for (int i = 1; i <= K; i++) {
            h1 = (h1 + h2) % M;
            h2 = (h2 +  i) % M;
            if (!op(buf, h1)) return 0;
        }
        return 1;
    }
};

struct RingIndex {
    int size, back, count, total;
    RingIndex(int S=0) : size(S), back(0), count(0), total(0) {}
    static int Wrap(int i, int s) { if ((i = i % s) < 0) i += s; return i; }
    static int WrapOver(int i, int s) { return i == s ? i : Wrap(i, s); }
    int Back() const { return Index(-1); }
    int Front() const { return Index(-count); }
    int Index(int i) const { return AbsoluteIndex(back + i); }
    int IndexOf(int i) const { return i - back - (i >= back ? size : 0); }
    int AbsoluteIndex(int i) const { return Wrap(i, size); }
    void IncrementSize(int n) { count = min(size, count + n); total += n; } 
    void DecrementSize(int n) { total = max(0, total - n); if (total < size) count = total; }
    void PopBack  (int n) { back = Index(-n); DecrementSize(n); }
    void PushBack (int n) { back = Index(+n); IncrementSize(n); }
    void PushFront(int n) { back = Index(-max(0, count+n-size)); IncrementSize(n); }
    void PopFront (int n) { DecrementSize(n); }
    void Clear() { back=count=total=0; }
};

template <class X> struct RingVector {
    RingIndex ring;
    vector<X> data;
    RingVector(int S=0) : ring(S), data(S) {}
    X&       operator[](int i)       { return data[ring.Index(i)]; }
    const X& operator[](int i) const { return data[ring.Index(i)]; }
    virtual int Size() const { return ring.count; }
    virtual int IndexOf(const X *v) const { return ring.IndexOf(v - &data[0]); }
    virtual X   *Back ()         { return &data[ring.Back()]; }
    virtual X   *Front()         { return &data[ring.Front()]; }
    virtual X   *PushBack ()     { ring.PushBack (1); return Back(); }
    virtual X   *PushFront()     { ring.PushFront(1); return Front(); }
    virtual void PopFront(int n) { ring.PopFront(n); }
    virtual void PopBack (int n) { ring.PopBack (n); }
    virtual void Clear() { ring.Clear(); }
};

struct RingBuf {
    RingIndex ring;
    int samplesPerSec, width, bytes;
    Allocator *alloc; char *buf; Time *stamp;
    ~RingBuf() { alloc->Free((void*)buf); alloc->Free((void*)stamp); }
    RingBuf(int SPS=0, int SPB=0, int Width=0, Allocator *Alloc=0) : samplesPerSec(0), width(0), bytes(0), alloc(Alloc?Alloc:Singleton<MallocAlloc>::Get()), buf(0), stamp(0)
    { if (SPS) Resize(SPS, X_or_Y(SPB, SPS), Width); }

    int Bucket(int n) const { return ring.AbsoluteIndex(n); }
    virtual void *Ind(int index) const { return (void *)(buf + index*width); }

    void Resize(int SPS, int SPB, int Width=0);
    enum WriteFlag { Peek=1, Stamp=2 };
    virtual void *Write(int writeFlag=0, Time timestamp=-1);

    int Dist(int indexB, int indexE) const;
    int Since(int index, int Next=-1) const;
    virtual void *Read(int index, int Next=-1) const;
    virtual Time ReadTimestamp(int index, int Next=-1) const;

    struct Handle : public Vec<float> {
        RingBuf *sb; int next, nlen;
        Handle() : sb(0), next(-1), nlen(-1) {}
        Handle(RingBuf *SB, int Next=-1, int Len=-1) : sb(SB), next((sb && Next != -1)?sb->Bucket(Next):-1), nlen(Len) {}
        virtual int Rate() const { return sb->samplesPerSec; }
        virtual int Len() const { return nlen >= 0 ? nlen : sb->ring.size; }
        virtual float *Index(int index) { return (float *)sb->Ind(index); }
        virtual float Ind(int index) const { return *(float *)sb->Ind(index); }
        virtual void Write(float v, int flag=0, Time ts=-1) { *(float *)(sb->Write(flag, ts)) = v; }
        virtual void Write(float *v, int flag=0, Time ts=-1) { *(float *)(sb->Write(flag, ts)) = *v; }
        virtual float Read(int index) const { return *(float *)sb->Read(index, next); }
        virtual float *ReadAddr(int index) const { return (float *)sb->Read(index, next); } 
        virtual Time ReadTimestamp(int index) const { return sb->ReadTimestamp(index, next); }
        void CopyFrom(const RingBuf::Handle *src);
    };

    template <typename X> struct HandleT : public Handle {
        HandleT() {}
        HandleT(RingBuf *SB, int Next=-1, int Len=-1) : Handle(SB, Next, Len) {}
        virtual float Ind(int index) const { return *(X *)sb->Ind(index); }
        virtual float Read(int index) const { return *(X *)sb->Read(index, next); }
        virtual void Write(float  v, int flag=0, Time ts=-1) { *(X *)(sb->Write(flag, ts)) =  v; }
        virtual void Write(float *v, int flag=0, Time ts=-1) { *(X *)(sb->Write(flag, ts)) = *v; }
    };

    struct DelayHandle : public Handle {
        int delay;
        DelayHandle(RingBuf *SB, int next, int Delay) : Handle(SB, next+Delay), delay(Delay) {}
        virtual float Read(int index) const {
            if ((index < 0 && index >= -delay) || (index > 0 && index >= Len() - delay)) return 0;
            return Handle::Read(index);
        }
    };

    struct WriteAheadHandle : public Handle {
        WriteAheadHandle(RingBuf *SB) : Handle(SB, SB->ring.back) {}
        virtual void Write(float v, int flag=0, Time ts=-1) { *(float*)Write(flag, ts) = v; }
        virtual void *Write(int flag=0, Time ts=-1) {
            void *ret = sb->Ind(next);
            if (flag & Stamp) sb->stamp[next] = ts ? ts : Now() * 1000;
            if (!(flag & Peek)) next = (next+1) % sb->ring.size;
            return ret;
        }
        void Commit() { sb->ring.back = sb->Bucket(next); }
    };

    template <class T=double> struct MatrixHandleT : public matrix<T> {
        RingBuf *sb; int next;
        MatrixHandleT(RingBuf *SB, int Next, int Rows) : sb(SB), next((sb && Next != -1)?sb->Bucket(Next):-1) { 
            matrix<T>::bytes = sb->width;
            matrix<T>::M = Rows?Rows:sb->ring.size;
            matrix<T>::N = matrix<T>::bytes/sizeof(T);
        }

        T             * row(int i)       { return (T*)sb->Read(i, next); }
        const T       * row(int i) const { return (T*)sb->Read(i, next); }
        Complex       *crow(int i)       { return (Complex*)sb->Read(i, next); }
        const Complex *crow(int i) const { return (Complex*)sb->Read(i, next); }
    };
    typedef MatrixHandleT<double> MatrixHandle;

    template <class T=double> struct RowMatHandleT {
        RingBuf *sb; int next;
        matrix<T> wrap;
        void Init() { wrap.M=1; wrap.bytes=sb->width; wrap.N=wrap.bytes/sizeof(T); }

        ~RowMatHandleT() { wrap.m=0; }
        RowMatHandleT() : sb(0), next(0) {}
        RowMatHandleT(RingBuf *SB) : sb(SB), next(-1) { Init(); }
        RowMatHandleT(RingBuf *SB, int Next) : sb(SB), next((sb && Next != -1)?sb->Bucket(Next):-1) { Init(); }

        matrix<T> *Ind(int index) { wrap.m = (double *)sb->Ind(index); return &wrap; }
        matrix<T> *Read(int index) { wrap.m = (double *)sb->Read(index, next); return &wrap; }
        double *ReadRow(int index) { wrap.m = (double *)sb->Read(index, next); return wrap.row(0); }
        matrix<T> *Write(int flag=0, Time ts=-1) { wrap.m = (double *)sb->Write(flag, ts); return &wrap; }
        Time ReadTimestamp(int index) const { return sb->ReadTimestamp(index, next); }
    };
    typedef RowMatHandleT<double> RowMatHandle;
};

struct ColMatPtrRingBuf : public RingBuf {
    Matrix *wrap; int col;
    ColMatPtrRingBuf(Matrix *m, int ci, int SPS=0) : wrap(m), col(ci)  {
        samplesPerSec = SPS;
        ring.size = m->M;
        width = sizeof(double);
        bytes = width * ring.size;
        ring.back = 0;
    }
    virtual void *Ind(int index) const { return &wrap->row(index)[col]; }
    virtual void *Read(int index, int Next=-1) const { return &wrap->row(index)[col]; }
    virtual void *Write(int writeFlag=0, Time timestamp=-1) { return &wrap->row(ring.back++)[col]; } 
    virtual Time ReadTimestamp(int index, int Next=-1) const { return index; }
};

struct CallbackList {
    bool dirty;
    vector<Callback> data;
    CallbackList() : dirty(0) {}
    int Size() const { return data.size(); }
    void Clear() { dirty=0; data.clear(); }
    void Run() { for (auto i = data.begin(); i != data.end(); ++i) (*i)(); Clear(); }
    void Add(const Callback &cb) { data.push_back(cb); dirty=1; }
    void Add(const CallbackList &cb) { data.insert(data.end(), cb.data.begin(), cb.data.end()); dirty=1; }
};
#define CallbackListAdd(cblist, ...) (cblist)->Add(bind(__VA_ARGS__))
#define CallbackListsAdd(cblists, ...) for(CallbackList **cbl=(cblists); *cbl; cbl++) CallbackListAdd(*cbl, __VA_ARGS__)

template <class X> struct AssetMapT {
    bool loaded;
    vector<X> vec;
    map<string, X*> amap;
    AssetMapT() : loaded(0) {}
    void Add(const X &a) { CHECK(!loaded); vec.push_back(a); }
    void Unloaded(X *a) { if (!a->name.empty()) amap.erase(a->name); }
    void Load(X *a) { a->parent = this; if (!a->name.empty()) amap[a->name] = a; a->Load(); }
    void Load() { CHECK(!loaded); for (int i=0; i<vec.size(); i++) Load(&vec[i]); loaded=1; }
    X *operator()(const string &an) { return FindOrNull(amap, an); }
};
typedef AssetMapT<     Asset>      AssetMap;
typedef AssetMapT<SoundAsset> SoundAssetMap;
typedef AssetMapT<MovieAsset> MovieAssetMap;

struct Serializable {
    struct Stream;

    virtual int Type() const = 0;
    virtual int Size() const = 0;
    virtual int HeaderSize() const = 0;
    virtual int In(const Stream *i) = 0;
    virtual void Out(Stream *o) const = 0;

    string ToString(unsigned short seq=0);
    void ToString(string *out, unsigned short seq=0);
    void ToString(char *buf, int len, unsigned short seq=0);

    struct Header {
        static const int size = 4;
        unsigned short id, seq;

        void Out(Stream *o) const;
        void In(const Stream *i);
    };

    bool HdrCheck(int content_len) { return content_len >= Header::size + HeaderSize(); }
    bool    Check(int content_len) { return content_len >= Header::size +       Size(); }
    bool HdrCheck(const Stream *is) { return HdrCheck(is->Len()); }
    bool    Check(const Stream *is) { return    Check(is->Len()); }
    int      Read(const Stream *is) { if (!HdrCheck(is)) return -1; return In(is); }

    struct Stream {
        virtual unsigned char  *N8()            = 0;
        virtual unsigned short *N16()           = 0;
        virtual unsigned       *N32()           = 0;
        virtual char           *Get(int size=0) = 0;
        virtual char           *End()           = 0;

        char *buf; int size; mutable int offset; mutable bool error;
        Stream(char *B, int S) : buf(B), size(S), offset(0), error(0) {}

        int Len() const { return size; }
        int Pos() const { return offset; };
        int Remaining() const { return size - offset; }
        const char *Start() const { return buf; }
        const char *Advance(int n=0) const { return Get(n); }
        const char           *End() const { return buf + size; }
        const unsigned char  *N8()  const { unsigned char  *ret = (unsigned char *)(buf+offset); offset += 1;   if (offset > size) { error=1; return 0; } return ret; }
        const unsigned short *N16() const { unsigned short *ret = (unsigned short*)(buf+offset); offset += 2;   if (offset > size) { error=1; return 0; } return ret; }
        const unsigned       *N32() const { unsigned       *ret = (unsigned      *)(buf+offset); offset += 4;   if (offset > size) { error=1; return 0; } return ret; }
        const char           *Get(int len=0) const { char  *ret = (char          *)(buf+offset); offset += len; if (offset > size) { error=1; return 0; } return ret; }

        void String(const char *buf, int len) { char *v = (char*)Get(len); if (v) memcpy(v, buf, len); }
        void String(const string &in) { char *v = (char*)Get(in.size()); if (v) memcpy(v, in.c_str(), in.size()); }

        void Write8 (const unsigned char  &in) { unsigned char  *v =                 N8();  if (v) *v = in; }
        void Write8 (const          char  &in) {          char  *v = (char*)         N8();  if (v) *v = in; }
        void Write16(const unsigned short &in) { unsigned short *v =                 N16(); if (v) *v = in; }
        void Write16(const          short &in) {          short *v = (short*)        N16(); if (v) *v = in; }
        void Write32(const unsigned int   &in) { unsigned int   *v =                 N32(); if (v) *v = in; }
        void Write32(const          int   &in) {          int   *v = (int*)          N32(); if (v) *v = in; }
        void Write32(const unsigned long  &in) { unsigned long  *v = (unsigned long*)N32(); if (v) *v = in; }
        void Write32(const          long  &in) {          long  *v = (long*)         N32(); if (v) *v = in; }

        void Ntohs(const unsigned short &in) { unsigned short *v =         N16(); if (v) *v = ntohs(in); }
        void Htons(const unsigned short &in) { unsigned short *v =         N16(); if (v) *v = htons(in); }
        void Ntohs(const          short &in) {          short *v = (short*)N16(); if (v) *v = ntohs(in); }
        void Htons(const          short &in) {          short *v = (short*)N16(); if (v) *v = htons(in); }
        void Ntohl(const unsigned int   &in) { unsigned int   *v =         N32(); if (v) *v = ntohl(in); }
        void Htonl(const unsigned int   &in) { unsigned int   *v =         N32(); if (v) *v = htonl(in); }
        void Ntohl(const          int   &in) {          int   *v = (int*)  N32(); if (v) *v = ntohl(in); }
        void Htonl(const          int   &in) {          int   *v = (int*)  N32(); if (v) *v = htonl(in); }

        void Htons(unsigned short *out) const { const unsigned short *v =         N16(); *out = v ? htons(*v) : 0; }
        void Ntohs(unsigned short *out) const { const unsigned short *v =         N16(); *out = v ? ntohs(*v) : 0; }
        void Htons(         short *out) const { const          short *v = (short*)N16(); *out = v ? htons(*v) : 0; }
        void Ntohs(         short *out) const { const          short *v = (short*)N16(); *out = v ? ntohs(*v) : 0; }
        void Htonl(unsigned int   *out) const { const unsigned int   *v =         N32(); *out = v ? htonl(*v) : 0; }
        void Ntohl(unsigned int   *out) const { const unsigned int   *v =         N32(); *out = v ? ntohl(*v) : 0; }
        void Htonl(         int   *out) const { const          int   *v = (int*)  N32(); *out = v ? htonl(*v) : 0; }
        void Ntohl(         int   *out) const { const          int   *v = (int*)  N32(); *out = v ? ntohl(*v) : 0; }

        void Read8 (unsigned char  *out) const { const unsigned char  *v =                 N8();  *out = v ? *v : 0; }
        void Read8 (         char  *out) const { const          char  *v = (char*)         N8();  *out = v ? *v : 0; }
        void Read16(unsigned short *out) const { const unsigned short *v =                 N16(); *out = v ? *v : 0; }
        void Read16(         short *out) const { const          short *v = (short*)        N16(); *out = v ? *v : 0; }
        void Read32(unsigned int   *out) const { const unsigned int   *v =                 N32(); *out = v ? *v : 0; }
        void Read32(         int   *out) const { const          int   *v = (int*)          N32(); *out = v ? *v : 0; }
        void Read32(unsigned long  *out) const { const unsigned long  *v = (unsigned long*)N32(); *out = v ? *v : 0; }
        void Read32(         long  *out) const { const          long  *v = (long*)         N32(); *out = v ? *v : 0; }
    };

    struct ConstStream : public Stream {
        ConstStream(const char *B, int S) : Stream((char*)B, S) {}
        char           *End()          { FATAL(this, ": ConstStream write"); return 0; }
        unsigned char  *N8()           { FATAL(this, ": ConstStream write"); return 0; }
        unsigned short *N16()          { FATAL(this, ": ConstStream write"); return 0; }
        unsigned       *N32()          { FATAL(this, ": ConstStream write"); return 0; }
        char           *Get(int len=0) { FATAL(this, ": ConstStream write"); return 0; }
    };

    struct MutableStream : public Stream {
        MutableStream(char *B, int S) : Stream(B, S) {}
        char           *End() { return buf + size; }
        unsigned char  *N8()  { unsigned char  *ret = (unsigned char *)(buf+offset); offset += 1;   if (offset > size) { error=1; return 0; } return ret; }
        unsigned short *N16() { unsigned short *ret = (unsigned short*)(buf+offset); offset += 2;   if (offset > size) { error=1; return 0; } return ret; }
        unsigned       *N32() { unsigned       *ret = (unsigned      *)(buf+offset); offset += 4;   if (offset > size) { error=1; return 0; } return ret; }
        char           *Get(int len=0) { char  *ret = (char          *)(buf+offset); offset += len; if (offset > size) { error=1; return 0; } return ret; }
    };
};

struct ProtoHeader {
    int flag, len; 
    ProtoHeader() : len(0) { SetFlag(0); }
    ProtoHeader(int f) : len(0) { SetFlag(f); }
    ProtoHeader(const char *text) {
        memcpy(&flag, text, sizeof(int));
        memcpy(&len, text+sizeof(int), sizeof(int));
        Validate();
    }
    void Validate() const { if (((flag>>16)&0xffff) != magic) FATAL("magic check"); }
    void SetLength(int v) { Validate(); len = v; }
    void SetFlag(unsigned short v) { flag = (magic<<16) | v; }
    unsigned short GetFlag() const { return flag & 0xffff; }
    static const int size = sizeof(int)*2, magic = 0xfefe;
};

/* file types */

struct VersionedFileName {
    const char *dir, *_class, *var;
    VersionedFileName(const char *D=0, const char *C=0, const char *V=0) : dir(D), _class(C), var(V) {}
};

struct ProtoFile {
    File *file; int read_offset, write_offset; bool done;
    ProtoFile(const char *fn=0) : file(0) { Open(fn); }
    ~ProtoFile() { delete file; }
    bool Opened() { return file && file->Opened(); }
    void Open(const char *fn);
    int Add(const Proto *msg, int status);
    bool Update(int offset, const ProtoHeader *ph, const Proto *msg);
    bool Update(int offset, int status);
    bool Get(Proto *out, int offset, int status=-1);
    bool Next(Proto *out, int *offsetOut=0, int status=-1);
    bool Next(ProtoHeader *hdr, Proto *out, int *offsetOut=0, int status=-1);
};

struct StringFile {
    vector<string> *F; string H;
    StringFile() { Clear(); }
    StringFile(vector<string> *f, const string &h=string()) : F(f), H(h) {}
    ~StringFile() { delete F; }

    void Clear() { F=0; H.clear(); }
    void Print(const string &name, bool nl=1);
    int Lines() const { return F ? F->size() : 0; }
    string Line(int i) const { return (F && i < F->size()) ? (*F)[i] : ""; }
    void AssignTo(vector<string> **Fo, string *Ho) { if (Fo) *Fo=F; if (Ho) *Ho=H; Clear(); }

    int ReadVersioned (const VersionedFileName &fn, int iter=-1);
    int WriteVersioned(const VersionedFileName &fn, int iter, const string &hdr=string());
    int WriteVersioned(const char *D, const char *C, const char *V, int iter, const string &hdr=string())
    { return WriteVersioned(VersionedFileName(D, C, V), iter, hdr); }

    int Read(const string &path, int header=1);
    int Read(IterWordIter *word, int header);

    int Write(File         *file, const string &name);
    int Write(const string &path, const string &name);
    static int WriteRow(File *file, const string &rowval);

    static int Read(const string &fn, vector<string> **F, string *H)
    { StringFile f; int ret=f.Read(fn); f.AssignTo(F, H); return ret; }
    static int ReadVersioned(const VersionedFileName &fn, vector<string> **F, string *H, int iter=-1)
    { StringFile f; int ret=f.ReadVersioned(fn); f.AssignTo(F, H); return ret; }
    static int ReadVersioned(const char *D, const char *C, const char *V, vector<string> **F, string *H, int iter=-1)
    { return ReadVersioned(VersionedFileName(D, C, V), F, H, iter); }
};

struct SettingsFile {
    static const char *VarType() { return "string"; }
    static const char *VarName() { return "settings"; }
    static const char *Separator() { return " = "; }

    static int Read(const string &dir, const string &name);
    static int Write(const vector<string> &fields, const string &dir, const string &name);
};

struct MatrixFile {
    struct Header { enum { NONE=0, DIM_PLUS=1, DIM=2 }; };
    struct BinaryHeader{ int magic, M, N, name, transcript, data, unused1, unused2; };

    Matrix *F; string H;
    MatrixFile() { Clear(); }
    MatrixFile(Matrix *f, const string &h=string()) : F(f), H(h) {}
    ~MatrixFile() { delete F; }

    void Clear() { F=0; H.clear(); }
    const char *Text() { return H.c_str(); }
    void AssignTo(Matrix **Fo, string *Ho) { if (Fo) *Fo=F; if (Ho) *Ho=H; Clear(); }

    int ReadVersioned       (const VersionedFileName &fn, int iteration=-1);
    int WriteVersioned      (const VersionedFileName &fn, int iteration);
    int WriteVersionedBinary(const VersionedFileName &fn, int iteration);
    int WriteVersioned(const char *D, const char *C, const char *V, int iter)
    { return WriteVersioned(VersionedFileName(D, C, V), iter); }
    int WriteVersionedBinary(const char *D, const char *C, const char *V, int iter)
    { return WriteVersionedBinary(VersionedFileName(D, C, V), iter); }

    int Read(IterWordIter *word, int header=1);
    int Read(const string &path, int header=1, int (*IsSpace)(int)=0);
    int ReadBinary(const string &path);

    int Write      (File         *file, const string &name);
    int Write      (const string &path, const string &name);
    int WriteBinary(File         *file, const string &name);
    int WriteBinary(const string &path, const string &name);

    static string Filename(const VersionedFileName &fn, const string &suf, int iter) { return Filename(fn._class, fn.var, suf, iter); }
    static string Filename(const string &_class, const string &var, const string &suffix, int iteration);
    static int FindHighestIteration(const VersionedFileName &fn, const string &suffix);
    static int FindHighestIteration(const VersionedFileName &fn, const string &suffix1, const string &suffix2);
    static int ReadHeader    (IterWordIter *word, string *hdrout);
    static int ReadDimensions(IterWordIter *word, int *M, int *N);
    static int WriteHeader      (File *file, const string &name, const string &hdr, int M, int N);
    static int WriteBinaryHeader(File *file, const string &name, const string &hdr, int M, int N);
    static int WriteRow         (File *file, const double *row, int N, bool lastrow=0);

    static int Read(IterWordIter *fd, Matrix **F, string *H)
    { MatrixFile f; int ret=f.Read(fd); f.AssignTo(F, H); return ret; }
    static int Read(const string &fn, Matrix **F, string *H)
    { MatrixFile f; int ret=f.Read(fn); f.AssignTo(F, H); return ret; }
    static int ReadVersioned(const VersionedFileName &fn, Matrix **F, string *H, int iter=-1)
    { MatrixFile f; int ret=f.ReadVersioned(fn); f.AssignTo(F, H); return ret; }
    static int ReadVersioned(const char *D, const char *C, const char *V, Matrix **F, string *H=0, int iter=-1)
    { return ReadVersioned(VersionedFileName(D, C, V), F, H, iter); }
};

struct MatrixArchiveOut {
    File *file;
    MatrixArchiveOut(const string &name=string());
    ~MatrixArchiveOut();

    void Close();
    int Open(const string &name);
    int Write(Matrix*, const string &hdr, const string &name);
    int Write(const MatrixFile *f, const string &name) { return Write(f->F, f->H, name); }
};

struct MatrixArchiveIn {
    IterWordIter *file; int index;
    MatrixArchiveIn(const string &name=string());
    ~MatrixArchiveIn();

    void Close();
    int Open(const string &name);
    int Read(Matrix **out, string *hdrout);
    int Read(MatrixFile *f) { return Read(&f->F, &f->H); }
    int Skip();
    string Filename();
    static int Count(const string &name);
};

template <class X, void (*Assign)(double *, X), bool (*Equals)(const double*, X)>
struct HashMatrixT {
    static double *Get(Matrix *map, X hash, int VPE) {
        double *hashrow = map->row(hash % map->M);
        for (int k=0, l=map->N/VPE; k<l; k++) if (Equals(&hashrow[k*VPE], hash)) return &hashrow[k*VPE];
        return 0;
    }
    static const double *Get(const Matrix *map, X hash, int VPE) {
        const double *hashrow = map->row(hash % map->M);
        for (int k=0, l=map->N/VPE; k<l; k++) if (Equals(&hashrow[k*VPE], hash)) return &hashrow[k*VPE];
        return 0;
    }
    static double *Set(Matrix *map, X hash, int VPE) {
        long long ind = hash % map->M;
        double *hashrow = map->row(ind);
        for (int k=0; k<map->N/VPE; k++) {
            int hri = k*VPE;
            if (hashrow[hri]) {
                if (Equals(&hashrow[hri], hash)) { ERROR("hash collision or duplicate insert ", hash); break; }
                continue;
            }
            Assign(&hashrow[hri], hash);
            return &hashrow[hri];
        }
        return 0;
    }
    static double *SetBinary(File *lf, int M, int N, int hdr_size, X hash, int VPE, double *hashrow) {
        long long ind = hash % M, row_size = N * sizeof(double), offset = hdr_size + ind * row_size, ret;
        if ((ret = lf->Seek(offset, File::Whence::SET)) != offset) { ERROR("seek: ", offset,   " != ", ret); return 0; } 
        if ((ret = lf->Read(hashrow, row_size))       != row_size) { ERROR("read: ", row_size, " != ", ret); return 0; }

        for (int k=0; k<N/VPE; k++) {
            int hri = k*VPE;
            if (hashrow[hri]) {
                if (Equals(&hashrow[hri], hash)) { ERROR("hash collision or duplicate insert ", hash); break; }
                continue;
            }
            int hri_offset = offset + hri * sizeof(double);
            if ((ret = lf->Seek(hri_offset, File::Whence::SET)) != hri_offset) { ERROR("seek: ", hri_offset, " != ", ret); return 0; } 
            Assign(&hashrow[hri], hash);
            return &hashrow[hri];
        }
        return 0;
    }
    static void SetBinaryFlush(LocalFile *lf, int VPE, const double *hashrow) {
        int write_size = VPE * sizeof(double);
        if (lf->Write(hashrow, write_size) != write_size) ERROR("read: ", write_size);
    }
};

struct HashMatrixF {
    static void Assign(/**/  double *hashrow, unsigned hash) { if (1) hashrow[0] =  hash; }
    static bool Equals(const double *hashrow, unsigned hash) { return hashrow[0] == hash; }
};
struct HashMatrix : public HashMatrixT<unsigned, &HashMatrixF::Assign, &HashMatrixF::Equals> {};

struct HashMatrix64F {
    static void Assign(double *hashrow, unsigned long long hash) {
        hashrow[0] = static_cast<unsigned>(hash>>32);
        hashrow[1] = static_cast<unsigned>(hash&0xffffffff);
    }
    static bool Equals(const double *hashrow, unsigned long long hash) {
        return hashrow[0] == static_cast<unsigned>(hash>>32) &&
               hashrow[1] == static_cast<unsigned>(hash&0xffffffff);
    }
};
struct HashMatrix64 : public HashMatrixT<unsigned long long, &HashMatrix64F::Assign, &HashMatrix64F::Equals> {};

}; // namespace LFL

#include "lfapp/tree.h"

#endif // __LFL_LFAPP_LFTYPES_H__
