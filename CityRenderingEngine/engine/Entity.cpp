#include "Entity.h"

Entity::Entity(void) {
    this->childEntities = new std::vector<Entity*>();
    physicalBody = new PhysicalBody(this, 0, Vector3());
    modelMatrix = new Matrix4();
    modelMatrix->toIdentity();
    shader = nullptr;
    model = nullptr;
    parent = nullptr;
    position = Vector3();
    rotation = Vector3();
    scale = Vector3(1, 1, 1);
}

Entity::Entity(const Entity &copy) {
    this->modelMatrix = new Matrix4(*(copy.modelMatrix));
    this->shader = new Shader(*(copy.shader));
    this->parent = new Entity(*(copy.parent));
    this->childEntities = new std::vector<Entity*>(*(copy.childEntities));
    this->model = new Model(*(copy.model));
    this->shader = copy.shader;
    this->physicalBody = new PhysicalBody(*(copy.physicalBody));
    this->position = Vector3(copy.position);
    this->rotation = Vector3(copy.rotation);
    this->scale = Vector3(copy.scale);
}

Entity::Entity(Vector3 position, Vector3 rotation, Vector3 scale) {
    this->childEntities = new std::vector<Entity*>();
    this->position = Vector3(position);
    this->rotation = Vector3(rotation);
    this->scale = Vector3(scale);
    modelMatrix = new Matrix4();
    modelMatrix->toIdentity();
    model = nullptr;
    shader = nullptr;
    parent = nullptr;
    
}

Entity::~Entity(void) {
    if (parent != NULL) {
        parent->removeChild(this);
    }
    delete modelMatrix;
    modelMatrix = nullptr;
    delete childEntities;
    childEntities = nullptr;
    shader = nullptr;
    delete physicalBody;
    physicalBody = nullptr;
}

Entity &Entity::operator=(const Entity &other) {
    *(this->modelMatrix) = *(other.modelMatrix);
    *(this->model) = *(other.model);
    this->shader = other.shader;
    if (parent != NULL) {
        if (other.parent == NULL) {
            this->parent = NULL;
        } else {
            *(this->parent) = *(other.parent);
        }
    } else {
        if (other.parent != NULL) {
            this->parent = new Entity(*(other.parent));
        }
    }
    *(this->childEntities) = *(other.childEntities);
    *(this->physicalBody) = *(other.physicalBody);
    this->position = Vector3(other.position);
    this->rotation = Vector3(other.rotation);
    this->scale = Vector3(other.scale);
    return *this;
}

void Entity::onMouseMoved(Vector2 &position, Vector2 &amount) {
    for (std::vector<Entity*>::iterator it = childEntities->begin(); it != childEntities->end(); ++it) {
        (*it)->onMouseMoved(position, amount);
    }
}

void Entity::onMouseClick(Uint8 button, Vector2 &position) {
    for (std::vector<Entity*>::iterator it = childEntities->begin(); it != childEntities->end(); ++it) {
        (*it)->onMouseClick(button, position);
    }
}

void Entity::onMouseDoubleClick(Uint8 button, Vector2 &position) {
    for (std::vector<Entity*>::iterator it = childEntities->begin(); it != childEntities->end(); ++it) {
        (*it)->onMouseDoubleClick(button, position);
    }
}

void Entity::onMouseButtonDown(Uint8 button, Vector2 &position) {
    for (std::vector<Entity*>::iterator it = childEntities->begin(); it != childEntities->end(); ++it) {
        (*it)->onMouseButtonDown(button, position);
    }
}

void Entity::onMouseButtonUp(Uint8 button, Vector2 &position) {
    for (std::vector<Entity*>::iterator it = childEntities->begin(); it != childEntities->end(); ++it) {
        (*it)->onMouseButtonUp(button, position);
    }
}

/*
 * The parameter amount is positive when the wheel is scrolled away from the player,
 * and negative if scrolled toward the player.
 */
void Entity::onMouseWheelScroll(int amount) {
    for (std::vector<Entity*>::iterator it = childEntities->begin(); it != childEntities->end(); ++it) {
        (*it)->onMouseWheelScroll(amount);
    }
}

void Entity::onKeyPress(SDL_Keysym key) {
    for (std::vector<Entity*>::iterator it = childEntities->begin(); it != childEntities->end(); ++it) {
        (*it)->onKeyPress(key);
    }
}

void Entity::onKeyDown(SDL_Keysym key) {
    for (std::vector<Entity*>::iterator it = childEntities->begin(); it != childEntities->end(); ++it) {
        (*it)->onKeyDown(key);
    }
}

void Entity::onKeyUp(SDL_Keysym key) {
    for (std::vector<Entity*>::iterator it = childEntities->begin(); it != childEntities->end(); ++it) {
        (*it)->onKeyUp(key);
    }
}


void Entity::calculateModelMatrix(Vector3 addPos, Vector3 addRot, Vector3 addSiz, bool pDiff, bool rDiff, bool sDiff) {
    pDiff = pDiff || position != lastPosition;
    if (pDiff) {
        // Position changed here or in the parent, update it in the matrix.
        modelMatrix->setPositionVector(position + addPos);
        lastPosition = Vector3(position);
    }
    rDiff = rDiff || rotation != lastRotation;
    sDiff = sDiff || scale != lastScale;
    if (rDiff || sDiff) {
        // Rotation or Scale changed, the whole matrix needs to be recalculated.
        Vector3 rot = (rotation + addRot);
        Matrix4 rotationMatrix =
            Matrix4::Rotation(rot.x, Vector3(1, 0, 0)) *
            Matrix4::Rotation(rot.y, Vector3(0, 1, 0)) *
            Matrix4::Rotation(rot.z, Vector3(0, 0, 1));
        *modelMatrix = Matrix4::Translation(position + addPos) * rotationMatrix * Matrix4::Scale(scale * addSiz);
        lastRotation = Vector3(rotation);
        lastScale = Vector3(scale);
    }
    // Do the same for all the children
    for(std::vector<Entity*>::iterator it = childEntities->begin(); it != childEntities->end(); ++it) {
        (*it)->calculateModelMatrix(position + addPos, rotation + addRot, scale + addSiz, pDiff, rDiff, sDiff);
    }
}

void Entity::update(float millisElapsed) {
    // Only update the matrix if this entity isn't a child, because that function also update all children's matrices.
    if (parent == nullptr) {
        calculateModelMatrix(Vector3(), Vector3(), Vector3(1, 1, 1), false, false, false);
    }
    for (std::vector<Entity*>::iterator it = childEntities->begin(); it != childEntities->end(); ++it) {
        (*it)->update(millisElapsed);
    }
}

void Entity::draw(float millisElapsed) {
    if (model != nullptr) {
        model->draw();
    }
    for (std::vector<Entity*>::iterator it = childEntities->begin(); it != childEntities->end(); ++it) {
        (*it)->draw(millisElapsed);
    }

    // If debug mode enabled, draw the collision spheres to track their position and check if collisions are correct
    bool debug = false;
    if (debug) {
        //GLuint program = GameApp::getInstance()->getDefaultShader()->getShaderProgram();
        Model *sphere = (Model*) Model::getOrCreate("SPHERE_MESH", "resources/models/sphere.mdl");
        if (sphere != NULL) {
            //std::vector<CollisionBody*> *colBodies = physicalBody->getCollisionBodies();
            //for (unsigned i = 0; i < min(colBodies->size(), 2); i++) {
            //	CollisionBody *colBody = (*colBodies)[i];
            //	if (colBody->getType() == BodyType::SPHERE) {
            //		Vector3 calcPos = Vector3(*(colBody->getAbsolutePosition()));
            //		Vector3 calcSiz = Vector3(colBody->getRadius(), colBody->getRadius(), colBody->getRadius());

            //		// Now that we calculated the final attributes, build the matrix
            //		Matrix4 colModelMatrix = Matrix4::Translation(calcPos) * Matrix4::Scale(calcSiz);
            //		glUniformMatrix4fv(glGetUniformLocation(program, "modelMatrix"), 1, false, (float*) &colModelMatrix);
            //		sphere->draw();
            //	}
            //}
        }
    }

}

void Entity::addChild(Entity *child) {
    if (child) {
        // If we don't have space to store the entity, make some!
        // I set this if to >= to always have an empty space in the array, just in case
        if ((childEntities->size() + 1) >= childEntities->capacity()) {
            childEntities->reserve(childEntities->capacity() + 5);
        }
        childEntities->emplace_back(child);
        child->parent = this;
    }
}

void Entity::removeChild(Entity *child) {
    if (child) {
        childEntities->erase(std::remove(childEntities->begin(), childEntities->end(), child), childEntities->end());
        // If vector becomes too big, shrink it
        if (childEntities->size() + 10 < childEntities->capacity()) {
            childEntities->shrink_to_fit();
        }
        child->parent = NULL;
    }
}

void Entity::makeOrphan() {
    if (parent) {
        parent->removeChild(this);
    }
}

std::vector<Entity*> Entity::getAllChildren(Entity *entity) {
    std::vector<Entity*> children = std::vector<Entity*>();
    children.emplace_back(entity);
    unsigned size = entity->childEntities->size();
    for (unsigned i = 0; i < size; i++) {
        if ((unsigned) entity->childEntities != 0xcdcdcd01 && (unsigned) entity->childEntities != 0xfeeefeee) {
            std::vector<Entity*> grandChildren = Entity::getAllChildren((*(entity->childEntities))[i]);
            children.insert(children.end(), grandChildren.begin(), grandChildren.end());
        }
    }
    return children;
}