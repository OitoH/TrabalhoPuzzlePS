
class BbTree {
	struct node {
		puzzle infos;
		int father;
		struct node *pointer[4]; /* up, down, left, right */
	};

private:
	struct node *root, *iterator;

public:
	BbTree(puzzle infos){
		root = allocNode(NULL);
		root.father = -1;
		root.infos = infos;
		iterator = NULL;
	}

	public void startDeathRide() {
		iterator = root;
		while(true /*condicao?*/){
			if(exploreNode()){
				iterator = getFavoriteChild();
			}
			else{
				backtrack();
			}
		}
	}

private:
	int exploreNode() {
		struct node aux = *iterator;
		if(iterator.father != 0 && aux.infos.move_zero('u') == 0){
			iterator.pointer[0] = allocNode();
			iterator.pointer[0].father = 0;
			iterator.pointer[0].infos = aux.infos;
		}
		aux = *iterator;
		if(iterator.father != 1 && aux.infos.move_zero('d') == 0){
			iterator.pointer[1] = allocNode();
			iterator.pointer[1].father = 1;
			iterator.pointer[1].infos = aux.infos;
		}
		aux = *iterator;
		if(iterator.father != 2 && aux.infos.move_zero('l') == 0){
			iterator.pointer[2] = allocNode();
			iterator.pointer[2].father = 2;
			iterator.pointer[2].infos = aux.infos;
		}
		aux = *iterator;
		if(iterator.father != 3 && aux.infos.move_zero('r') == 0){
			iterator.pointer[3] = allocNode();
			iterator.pointer[3].father = 3;
			iterator.pointer[3].infos = aux.infos;
		}

		return 0;
	}

	struct node* getFavoriteChild() {
		int i, best = 99999;
		struct node *aux = NULL;

		for (i=0; i<4; i++)
			if(iterator.pointer[i] != NULL && iterator.infos.manhattan_dist() < best){
				best = iterator.infos.manhattan_dist();
				aux = iterator.pointer[i];
			}

		return aux;
	}

	void backtrack() {
		iterator = iterator.pointer[4];
		if (getFavoriteChild == NULL){
			backtrack();
		}

		return;
	}

	struct node* allocNode(){
		struct node *aux;
		aux = (struct node) malloc(sizeof(struct node));
		aux.pointer[0] = NULL;
		aux.pointer[1] = NULL;
		aux.pointer[2] = NULL;
		aux.pointer[3] = NULL;

		return aux;
	}
}
