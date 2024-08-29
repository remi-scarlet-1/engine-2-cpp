- [x] improve GLTF/GLB model loading ram usage
- [x] improve GLTF/GLB model loading speed
- [x] refactor model  
Model, Mesh & Primitive header is good but implementation is hacked together. Improved ram usage and loading speed by implementing mock image load functions. (still ram wasting but whatever)
This needs to be reimplemented and maybe another lighter lib then TinyGLTF...  
    
- [x] implement test rendering
- [x] implement camera  
  
- [x] implement time module (delta time)
- [x] implement filesystemHelper (getExecutableDir, getAssetDir, readFileToString)
				
- [ ] clean up main.cpp
- [ ] render callback
- [ ] renderer
  
- [x] figure out why primitives (submeshes) seem broken when rendered. Validate loaded intyGLTF data
- [ ] maybe rework model loading, cause its turbo scuffed
- [ ] general refactor & docs
