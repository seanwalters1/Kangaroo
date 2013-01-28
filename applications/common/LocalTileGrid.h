#include <Eigen/Eigen>
#include <list>
#include <vector>
#include <sophus/se3.h>

#include <kangaroo/ImageIntrinsics.h>

struct Tile
{
    // vertices a,b,c,d,e,f,g,h
    // #) face - opposite face
    // 0) abcd - 3) efgh    -x -> +x
    // 1) aehd - 4) bfgc    -y -> +y
    // 2) abfe - 5) dcgh    -z -> +z

    Tile()
    {
        // Clear vertex references
        for(int i=0; i<8; ++i) {
            verts[i] = 0;
        }

        // Clear adjacency face references
        for(int i=0; i<6; ++i) {
            adj[i] = 0;
        }
    }
    
    Tile* dx(int i) {
        return (i==0) ? this : ( i<0 ? adj[0] : adj[3] );
    }

    Tile* dy(int i) {
        return (i==0) ? this : ( i<0 ? adj[1] : adj[4] );
    }

    Tile* dz(int i) {
        return (i==0) ? this : ( i<0 ? adj[2] : adj[5] );
    }
    
    inline Tile* Face(int face)
    {
        return adj[face];
    }

    inline Tile* OppositeFace(int face)
    {
        return adj[(face+3) % 6];
    }

    void CanonicalExpansion(int face)
    {

    }

protected:
    // Pointers to shared vertices
    Eigen::Vector3d* verts[8];

    // Adjacency list of faces, ordered pairwise in opposites.
    Tile* adj[6];

    bool IntersectsWithFrustum(const Sophus::SE3 T_wc, int w, int h, Gpu::ImageIntrinsics K, float near, float far)
    {
        // TODO: implement
        return false;
    }
};

template<int Rad>
class LocalTileGrid
{
public:
    // Grid indices go from -Rad to +Rad inclusive.
    const size_t R = Rad;
    const size_t N = Rad*2 + 1;
    
    LocalTileGrid()
    {
        Clear();
    }
    
    LocalTileGrid(Tile* center_tile)
    {
        Clear();
        Get(0,0,0) = center_tile;
        FillLocalGrid();
    }
    
    void Clear()
    {
        for(int i=0; i < N*N*N; ++i) {
            grid[i] = nullptr;
        }        
    }
    
    inline bool InBounds(int i)
    {
        return -Rad <= i && i <= Rad;
    }
        
    inline Tile*& Get(int x, int y, int z)
    {
        return grid[N*N* (z+Rad) + N*(y+Rad) + (x+Rad)];        
    }

    inline Tile* operator()(int x, int y, int z)
    {
        return Get(x,y,z);
    }
        
    void Fill(int x, int y, int z)
    {
        Tile*& tile = Get(x,y,z);
        
        for(int i=-1; i<=1 && tile == nullptr; i+=2) {
            if(InBounds(x+i) && Get(x+i,y,z) != nullptr && Get(x+i,y,z)->dx(-i) != nullptr) {
                tile = Get(x+i,y,z)->dx(-i);
            } else if(InBounds(y+i) && Get(x,y+i,z) != nullptr && Get(x,y+i,z)->dy(-i) != nullptr) {
                tile = Get(x,y+i,z)->dy(-i);
            } else if(InBounds(z+i) && Get(x,y,z+i) != nullptr && Get(x,y,z+i)->dz(-i) != nullptr) {
                tile = Get(x,y,z+i)->dz(-i);
            }
        }
    }
    
    void FillLocalGrid()
    {
        for(int r=0; r< Rad; ++r) {
            for(int z=-Rad; z<=Rad; ++z) {
                for(int y=-Rad; y<=Rad; ++y) {
                    for(int x=-Rad; x<=Rad; ++x) {
                        Fill(x,y,z);   
                    }                    
                }
            }
        }
    }
    
protected:
    
    Tile* grid[N*N*N];
};
