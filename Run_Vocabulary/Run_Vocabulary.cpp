#include <iostream>
#include <Windows.h>
#include <cstring>
using namespace std;

struct vocab_tree
{
    char eng_word[30];
    char rus_word[30];
    int count;
    bool is_active;
    vocab_tree* left; //in original - words of the same letter
    vocab_tree* right; // in original - next letter of words

	vocab_tree()
	{
        left = NULL;
        right = NULL;
        count = 0;
        is_active = true;
	}
};


vocab_tree* new_words (int numb_words)
{
    vocab_tree* all_words = new vocab_tree[numb_words];

    for(int i=0;i<numb_words;i++)
    {
        cout << "Enter new english word: ";
        cin >> all_words[i].eng_word;
        cout << "Enter the russian translation: ";
        cin >> all_words[i].rus_word;
        cout << "Enter the counter value: ";
        cin >> all_words[i].count;
        cout << "___Processed...___" << endl;
    }
	
    return all_words;
}

void alph_sort(vocab_tree vocab_arr[], int numb_of_words)
{
    for (int i = 0;i < numb_of_words - 1;i++)
    {
        for (int j = i; j < numb_of_words;j++)
        {
            if (strcmp(vocab_arr[i].eng_word, vocab_arr[j].eng_word) > 0)
            {
                swap(vocab_arr[i], vocab_arr[j]);
            }
        }
    }
}

vocab_tree* standard_vocabulary_tree(vocab_tree sorted_tree_elements[], int numb_words)
{
    vocab_tree* start_point = sorted_tree_elements;
    vocab_tree* current_letter = sorted_tree_elements;

    for (int i = 1;i < numb_words;i++)
    {
        if (sorted_tree_elements[i].eng_word[0] == current_letter->eng_word[0])
        {
            sorted_tree_elements[i - 1].left = &sorted_tree_elements[i];
        }
        else
        {
            current_letter->right = &sorted_tree_elements[i];
            current_letter = &sorted_tree_elements[i];
        }
    }

    return start_point;
}

vocab_tree* find_word(char* eng, vocab_tree* start_node)
{
    vocab_tree* current_node = start_node;
    vocab_tree* current_right_node = start_node;
    while (current_right_node != NULL)
    {
        bool if_right_word = true; //checking if the words are the same:
    	if(strlen(eng) != strlen(current_node->eng_word))
            if_right_word = false;
        else
        {
            for (int i_checked_symb = 0;i_checked_symb < strlen(eng);i_checked_symb++)
            {
                    if (eng[i_checked_symb]!= current_node->eng_word[i_checked_symb])
                    {
                        if_right_word = false;
                        break;
                    }
                if (!if_right_word)
                    break;
            }
        }
        if(if_right_word && current_node->is_active) //if the word was found
        {
            current_node->count++;
            return current_node;
            break;
        }
        if (current_node->left != NULL)
        {
            current_node = current_node->left;
        }
        else
        {
            current_right_node = current_right_node->right;
            current_node = current_right_node;
        }
    }
    cout << "Such word wasn't found! " << endl;
    return NULL;
}

void show_translation(char* eng, vocab_tree* start)
{
    vocab_tree* res_translation_node = find_word(eng, start);
	if(res_translation_node!=NULL)
    cout << "In russian it is: " << res_translation_node->rus_word << endl;
}

void help_find_coordinates_of_max_node(vocab_tree* start, int& left_coord, int& right_coord); //left_coord and right_coord - results for the max count node

void help_delete_node_with_coord(vocab_tree* start_node, int right_coord, int left_coord);

vocab_tree* help_find_node_with_coord(vocab_tree* start_node, int right_coord, int left_coord);

bool help_is_tree_not_empty(vocab_tree* start_node);

void copying_tree_node(vocab_tree* from_node, vocab_tree* to_node)
{
    for(int i_word = 0;i_word<30;i_word++)
    {
        to_node->eng_word[i_word] = from_node->eng_word[i_word];
        to_node->rus_word[i_word] = from_node->rus_word[i_word];
    }
    to_node->count = from_node->count;
    to_node->is_active = from_node->is_active;
}

vocab_tree* construct_new_tree(vocab_tree* orig_start)
{
    vocab_tree* new_tree_start = new vocab_tree;
    vocab_tree* current_new_node = new_tree_start;

    vocab_tree* next_new_node;
	
   while(help_is_tree_not_empty(orig_start))
   {
		int max_right_ind, max_left_ind;
        help_find_coordinates_of_max_node(orig_start, max_left_ind, max_right_ind);
        vocab_tree* received_node = help_find_node_with_coord(orig_start, max_right_ind, max_left_ind);
        copying_tree_node(received_node, current_new_node);
        help_delete_node_with_coord(orig_start, max_right_ind, max_left_ind);

        next_new_node = new vocab_tree;
        current_new_node->right = next_new_node;

        current_new_node = next_new_node;
   }

   current_new_node->is_active = false; //eliminating of last 'empty' node on new tree
	
    return new_tree_start;
}

void display_tree_values(vocab_tree* start_node)
{
    vocab_tree* current_node = start_node;
    vocab_tree* current_right_node = start_node;
    while (current_right_node != NULL)
	{
    	if(current_node->is_active)
        cout << current_node->eng_word << " - " << current_node->rus_word << " #" << current_node->count << endl;
		if(current_node->left!=NULL)
		{
            current_node = current_node->left;
		}
        else
        {
            current_right_node = current_right_node->right;
            current_node = current_right_node;
        }
	}
}

int main()
{
    SetConsoleCP(1251); //for russian in console
    SetConsoleOutputCP(1251); //for russian in console

    cout << "WELCOME TO THE VOCABULARY!" << endl <<endl;
    cout << "(At first you should create a vocabulary) " << endl;
	
    int numb_words_general = 0;
    cout << "Enter the number of words you're ready to provide: ";
    cin >> numb_words_general;

    system("CLS");
    cout << "[SETTING UP THE VOCABULARY]" << endl;
    vocab_tree* array_words = new_words(numb_words_general);

    alph_sort(array_words, numb_words_general);

    int choice = -1;
	while(choice != 0)
	{
        system("CLS");
        cout << "MENU:" << endl;
        cout << "- Press '1' to display the content of vocabulary." << endl;
        cout << "- Press '2' to search for specific english word." << endl;
        cout << "- Press '3' to form the 'New' vocabulary." << endl;
        cout << "- Press '0' for Exit." << endl;

        cin >> choice;

        vocab_tree* orig_tree = standard_vocabulary_tree(array_words, numb_words_general);
		
		if(choice == 1)
		{
            system("CLS");
            cout << "THE CONTENT: " << endl;
            display_tree_values(orig_tree);
            cout << endl << endl;
            cout << "- Press '5' to return: ";
            cin >> choice;
		}
		if(choice == 2)
		{
            system("CLS");
            cout << "THE SEARCH: " << endl;
            char search_word[30];
            cout << "Enter the word to be translated: ";
            cin >> search_word;
            show_translation(search_word, orig_tree);
            cout << "- Press '5' to return: ";
            cin >> choice;
		}
		if(choice==3)
		{
            system("CLS");
            cout << "THE NEW VOCABULARY: " << endl << endl;
            vocab_tree* new_tree = construct_new_tree(orig_tree);
            cout << "The 'New' content: " << endl;
            display_tree_values(new_tree);
            cout << "- Press '0' to Exit: ";
            cin >> choice;
		}
	}
    	
	return 0;
}

void help_find_coordinates_of_max_node(vocab_tree* start, int& left_coord, int& right_coord) //left_coord and right_coord - results for the max count node
{
    vocab_tree* current_node = start;
    vocab_tree* current_right_node = start;

    int max_right_coordin = 0;
    int max_left_coordin = 0;
    int max_counter = 0;

    int init_coord_right = 0, init_coord_left = 0;
    while (current_node != NULL && current_right_node != NULL) //finding the max counter (+ 'coordinates' to this element)
    {
        if (current_node->is_active)
            if (current_node->count > max_counter)
            {
                max_counter = current_node->count;
                max_right_coordin = init_coord_right;
                max_left_coordin = init_coord_left;
            }
        if (current_node->left != NULL)
        {
            current_node = current_node->left;
            init_coord_left++;
        }
        else
        {
            current_right_node = current_right_node->right;
            current_node = current_right_node;
            init_coord_left = 0;
            init_coord_right++;
        }
    }
    left_coord = max_left_coordin;
    right_coord = max_right_coordin;
}

void help_delete_node_with_coord(vocab_tree* start_node, int right_coord, int left_coord)
{
    vocab_tree* current_node = start_node;

    for (int i = 0;i <= right_coord - 1;i++)
    {
        current_node = current_node->right;
    }
    for (int j = 0;j <= left_coord - 1;j++)
    {
        current_node = current_node->left;
    }
    current_node->is_active = false;

}

vocab_tree* help_find_node_with_coord(vocab_tree* start_node, int right_coord, int left_coord)
{
    vocab_tree* current_node = start_node;

    for (int i = 0;i <= right_coord - 1;i++)
    {
        current_node = current_node->right;
    }
    for (int j = 0;j <= left_coord - 1;j++)
    {
        current_node = current_node->left;
    }

    return current_node;
}

bool help_is_tree_not_empty(vocab_tree* start_node)
{
    bool not_empty = false;

    vocab_tree* current_node = start_node;
    vocab_tree* current_right_node = start_node;
    while (current_node != NULL && current_right_node != NULL)
    {
        if (current_node->is_active)
        {
            not_empty = true;
            break;
        }
        if (current_node->left != NULL)
        {
            current_node = current_node->left;
        }
        else
        {
            current_right_node = current_right_node->right;
            current_node = current_right_node;
        }
    }
    return not_empty;
}