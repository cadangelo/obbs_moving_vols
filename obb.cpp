#include <iostream>
#include <cstdlib>
#include <cstdio>

#include "moab/Core.hpp"
#include "moab/Interface.hpp"
#include "moab/Range.hpp"
#include "moab/AdaptiveKDTree.hpp"
#include "moab/ElemEvaluator.hpp"
#include "moab/GeomTopoTool.hpp"
#include "moab/OrientedBoxTreeTool.hpp"
#include "moab/CN.hpp"
#include "moab/SpatialLocator.hpp"
#include "moab/Util.hpp"
#include "moab/GeomUtil.hpp"
#include "MBTagConventions.hpp"

using namespace moab;
Core core;
Interface *MBI;
//Interface *MBI = &core;
//GeomTopoTool gtt(MBI);
GeomTopoTool *GTT;
OrientedBoxTreeTool *obb;

Tag geom_tag;
Tag geom_dim_tag;

Range get_children_by_dimension(EntityHandle parent, int desired_dimension)
{
  ErrorCode rval;
  Range all_children, desired_children;
  Range::iterator it;
  int actual_dimension;

  int negone = -1;
  rval = MBI->tag_get_handle( GEOM_DIMENSION_TAG_NAME, 1, MB_TYPE_INTEGER,
                                geom_tag);
//, MB_TAG_SPARSE|MB_TAG_CREAT,&negone);
  std::cout << "get tag handle fxn " << rval << std::endl;
  //MB_CHK_SET_ERR_RET_VAL(rval, "Failed to get handle");

  all_children.clear();
  rval = MBI->get_child_meshsets(parent, all_children);
  MB_CHK_SET_ERR_RET_VAL(rval, "Failed to get child meshsets", all_children);

  for ( it = all_children.begin() ; it != all_children.end() ; ++it)
    {
      rval = MBI->tag_get_data(geom_tag, &(*it), 1, &actual_dimension);
      MB_CHK_SET_ERR_RET_VAL(rval, "Failed to get geom tag from child meshset", all_children);
      if ( actual_dimension == desired_dimension )
        {
          desired_children.insert(*it);
        }
    }

  return desired_children;

}

ErrorCode delete_surf_trees(EntityHandle vol_root){

//std::vector<EntityHandle> children;
//ErrorCode rval = instance->get_child_meshsets( set, children, 0 );
//if (MB_SUCCESS != rval)
//  return rval;
//
//GTT->obb_tree()->createdTrees.erase( 
//  std::remove( GTT->obb_tree()->createdTrees.begin(), GTT->obb_tree()->createdTrees.end(), set ),
//  GTT->obb_tree()->createdTrees.end() );
//return MB_SUCCESS;
}

int main(int argc, char **argv){

ErrorCode rval;

MBI = new Core();

// get dim tag
int negone = -1;
rval = MBI->tag_get_handle( GEOM_DIMENSION_TAG_NAME, 1, MB_TYPE_INTEGER,
                              geom_tag);
//Load file into meshset 
EntityHandle fileset;
std::string filename = argv[1];
std::cout << filename<< std::endl;
rval = MBI->create_meshset(MESHSET_SET, fileset);
MB_CHK_SET_ERR(rval, "Error creating meshset.");
rval = MBI->load_file(filename.c_str(), &fileset);
MB_CHK_SET_ERR(rval, "Error loading file.");

//Build obb trees
GTT = new GeomTopoTool(MBI);
rval = GTT->construct_obb_trees(false);
MB_CHK_SET_ERR(rval, "Error constructing all trees.");
//EntityHandle modelSet;
//modelSet = GTT->get_root_model_set();

//Get all volumes
Range vols;
rval = GTT->get_gsets_by_dimension(3, vols);
std::cout << "num vols gsets " << vols.size() << std::endl;
Range surfs, surfs1;
rval = GTT->get_gsets_by_dimension(2, surfs1);
std::cout << "num surfs1 gsets " << surfs1.size() << std::endl;
int tag_dim;// = 3; 
rval = MBI->tag_get_data( geom_tag, &(*vols.begin()), 1, &tag_dim);
std::cout << "get data " << rval << " " << tag_dim<< std::endl;

int num_vols;
num_vols = GTT->num_ents_of_dim(3);
MB_CHK_SET_ERR(rval, "Error getting 3 ents.");
std::cout << "num vols in model set " << num_vols << std::endl;

//Get root of a volume obb
EntityHandle root, sroot;
rval = GTT->get_root(*vols.begin(), root);
MB_CHK_SET_ERR(rval, "Error getting root.");
std::cout << "root eh " << root << std::endl;

// Delete surface trees and root
//Range surfs;
//surfs = get_children_by_dimension(*vols.begin(), 2);
//std::cout << "num surfs " << surfs.size() << std::endl;
//
//Range::iterator it;
//for ( it = surfs.begin() ; it != surfs.end() ; ++it){
//  
//  rval = GTT->obb_tree()->delete_tree(*it);
//  MB_CHK_SET_ERR(rval, "Error removing surf tree.");
//  rval = GTT->remove_root(*it);
//  MB_CHK_SET_ERR(rval, "Error removing surf root.");
//
//}

// Unjoin vol tree
rval = GTT->obb_tree()->unjoin_trees(root);
MB_CHK_SET_ERR(rval, "Error unjoining vol tree.");
  
// Delete old vol obb tree
//rval = GTT->obb_tree()->delete_tree(*vols.begin());
//MB_CHK_SET_ERR(rval, "Error removing surf tree.");
//
//rval = GTT->get_gsets_by_dimension(2, surfs);
//std::cout << "num surfs gsets " << surfs.size() << std::endl;
//rval = GTT->get_root(*vols.begin(), sroot);
//MB_CHK_SET_ERR(rval, "Error finding vol root.");
//std::cout << "orig vol root eh " << sroot << std::endl;
//rval = GTT->remove_root(*vols.begin());
//MB_CHK_SET_ERR(rval, "Error removing vol root.");
//
//// Rebuild vol and surf tree
//std::cout << "vol eh " << *vols.begin() << std::endl;
//int tag_dim;// = 3; 
//rval = MBI->tag_get_data( geom_tag, &(*vols.begin()), 1, &tag_dim);
//std::cout << "get data " << rval << " " << tag_dim<< std::endl;

//rval = MBI->tag_set_data( geom_tag, &(*vols.begin()), 1, &tag_dim);
//std::cout << "set data " << rval << std::endl;
// Get all 3D elements in fileset
//rval = GTT->add_geo_set(*vols.begin(), tag_dim);
//MB_CHK_SET_ERR(rval, "Error adding geo set.");



//rval = GTT->construct_obb_tree(*vols.begin());
//MB_CHK_SET_ERR(rval, "Error building vol tree.");
//  rval = GTT->get_root(*vols.begin(), sroot);
//  MB_CHK_SET_ERR(rval, "Error finding vol root.");
//std::cout << "new vol root eh " << sroot << std::endl;



}
