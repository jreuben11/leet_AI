# Hugging Face Course Notebooks

This repository contains Jupyter notebooks from the Hugging Face NLP course, organized by chapter. These notebooks cover various aspects of natural language processing using the Transformers library.

## Chapter 1: Transformer Models

- [section3.ipynb](chapter1/section3.ipynb) - Exploring different transformer pipeline tasks including sentiment analysis, zero-shot classification, text generation, fill-mask, NER, question answering, summarization, and translation
- [section8.ipynb](chapter1/section8.ipynb) - Understanding bias and limitations in transformer models, demonstrating gender bias in BERT's fill-mask predictions

## Chapter 2: Using Transformers

- [section2_pt.ipynb](chapter2/section2_pt.ipynb) - Understanding the pipeline internals in PyTorch, breaking down tokenization, model inference, and post-processing steps
- [section3_pt.ipynb](chapter2/section3_pt.ipynb) - Working with models in PyTorch, including building configurations, loading pretrained models, and saving models
- [section4_pt.ipynb](chapter2/section4_pt.ipynb) - Understanding tokenizers in PyTorch, exploring tokenization, encoding, decoding, and saving tokenizers
- [section5_pt.ipynb](chapter2/section5_pt.ipynb) - Handling multiple sequences with padding, attention masks, and truncation in PyTorch
- [section6_pt.ipynb](chapter2/section6_pt.ipynb) - Putting it all together in PyTorch, combining tokenization, padding, truncation, and model inference

## Chapter 3: Fine-tuning a Pretrained Model

- [section2.ipynb](chapter3/section2.ipynb) - Processing data for training, loading GLUE datasets, tokenizing sentence pairs, and using data collators
- [section3.ipynb](chapter3/section3.ipynb) - Fine-tuning a model with the Trainer API, setting up training arguments, computing metrics, and evaluating results
- [section4.ipynb](chapter3/section4.ipynb) - Full training loop implementation with custom dataloaders, optimizer setup, learning rate schedulers, and Accelerate integration
- [section7.ipynb](chapter3/section7.ipynb) - Understanding overfitting and underfitting with different learning rates and dataset sizes on text classification tasks

## Chapter 4: Sharing Models and Tokenizers

- [section2_pt.ipynb](chapter4/section2_pt.ipynb) - Using pretrained models from different languages and domains, exploring CamemBERT for French NLP
- [section3_pt.ipynb](chapter4/section3_pt.ipynb) - Sharing pretrained models to the Hub, configuring git, authentication, and pushing models with TrainingArguments or manually

## Chapter 5: The Datasets Library

- [section2.ipynb](chapter5/section2.ipynb) - Loading datasets from local and remote sources, working with JSON, CSV, and compressed files
- [section3.ipynb](chapter5/section3.ipynb) - Slicing and dicing datasets with filtering, mapping, tokenization, concatenation, and saving processed datasets
- [section4.ipynb](chapter5/section4.ipynb) - Working with big data using streaming datasets to handle large corpora efficiently
- [section5.ipynb](chapter5/section5.ipynb) - Creating your own dataset by fetching GitHub issues via API and uploading to the Hub
- [section6_pt.ipynb](chapter5/section6_pt.ipynb) - Semantic search with FAISS in PyTorch, building embeddings and finding similar documents

## Chapter 6: The Tokenizers Library

- [section2.ipynb](chapter6/section2.ipynb) - Training a new tokenizer from an existing one on code datasets using BPE algorithm
- [section3_pt.ipynb](chapter6/section3_pt.ipynb) - Fast tokenizers' special powers including offset mapping, word IDs, and token classification features
- [section3b_pt.ipynb](chapter6/section3b_pt.ipynb) - Fast tokenizers in QA pipelines with context windowing, overflow handling, and answer extraction
- [section4.ipynb](chapter6/section4.ipynb) - Understanding normalization and pre-tokenization steps in different tokenizer types
- [section5.ipynb](chapter6/section5.ipynb) - Byte-Pair Encoding (BPE) tokenization algorithm implementation from scratch
- [section6.ipynb](chapter6/section6.ipynb) - WordPiece tokenization algorithm used by BERT, training and encoding examples
- [section7.ipynb](chapter6/section7.ipynb) - Unigram tokenization algorithm using probabilistic approach for subword segmentation
- [section8.ipynb](chapter6/section8.ipynb) - Building a tokenizer block by block using the tokenizers library with custom normalization, pre-tokenization, and post-processing

## Chapter 7: Main NLP Tasks

- [section2_pt.ipynb](chapter7/section2_pt.ipynb) - Token classification (NER) in PyTorch, aligning labels with tokens, using data collators, and evaluating with seqeval
- [section3_pt.ipynb](chapter7/section3_pt.ipynb) - Fine-tuning a masked language model in PyTorch on IMDB reviews with whole word masking
- [section4_pt.ipynb](chapter7/section4_pt.ipynb) - Translation task in PyTorch, fine-tuning MarianMT models on English-French parallel corpus
- [section5_pt.ipynb](chapter7/section5_pt.ipynb) - Summarization in PyTorch, fine-tuning mT5 on multilingual Amazon reviews with ROUGE evaluation
- [section6_pt.ipynb](chapter7/section6_pt.ipynb) - Training a causal language model from scratch in PyTorch, building GPT-2 for code generation
- [section7_pt.ipynb](chapter7/section7_pt.ipynb) - Question answering in PyTorch, fine-tuning BERT on SQuAD dataset with context windowing

## Chapter 8: How to Ask for Help

- [section2.ipynb](chapter8/section2.ipynb) - What to do when you get an error, debugging common issues with model loading and configuration
- [section3.ipynb](chapter8/section3.ipynb) - Asking for help on the forums with minimal reproducible examples
- [section4.ipynb](chapter8/section4.ipynb) - Debugging the training pipeline step by step, from data collation to model output
- [section5.ipynb](chapter8/section5.ipynb) - How to write a good issue report for open source projects

## Chapter 9: Building and Sharing Demos

- [section2.ipynb](chapter9/section2.ipynb) - Building your first Gradio demo with simple text generation interface
- [section3.ipynb](chapter9/section3.ipynb) - Understanding the Interface class with audio, image, and multi-input examples
- [section4.ipynb](chapter9/section4.ipynb) - Sharing demos with others using titles, descriptions, examples, and public links
- [section5.ipynb](chapter9/section5.ipynb) - Integrations with the Hugging Face Hub, loading models and Spaces programmatically
- [section6.ipynb](chapter9/section6.ipynb) - Advanced Interface features including chatbots, state management, and interpretation
- [section7.ipynb](chapter9/section7.ipynb) - Introduction to Blocks for building custom layouts with tabs, rows, and event handlers

## Chapter 10: Data Annotation with Argilla

- [section2.ipynb](chapter10/section2.ipynb) - Setting up your Argilla instance for data annotation workflows
- [section3.ipynb](chapter10/section3.ipynb) - Loading your dataset to Argilla with custom fields, questions, and label configurations
- [section5.ipynb](chapter10/section5.ipynb) - Using your annotated dataset by filtering, exporting to Hub, and loading back

## Chapter 11: Fine-tuning with TRL

- [section2.ipynb](chapter11/section2.ipynb) - Exploring chat templates with SmolLM2, applying conversation formatting and tokenization
- [section3.ipynb](chapter11/section3.ipynb) - Supervised fine-tuning with SFTTrainer on conversational datasets using chat templates
- [section4.ipynb](chapter11/section4.ipynb) - Fine-tuning LLMs with LoRA adapters for parameter-efficient training, merging, and deployment

## Chapter 13: Reinforcement Learning

- [grpo_finetune.ipynb](chapter13/grpo_finetune.ipynb) - Fine-tuning LLMs with GRPO (Group Relative Policy Optimization) using custom reward functions

## Additional Files

- [main.py](main.py) - Python script for running course examples
- [pyproject.toml](pyproject.toml) - Project dependencies and configuration
- [uv.lock](uv.lock) - Lock file for dependency management

## Getting Started

1. Install dependencies:
```bash
pip install transformers datasets evaluate accelerate
```

2. For GPU support, install PyTorch with CUDA:
```bash
pip install torch torchvision torchaudio --index-url https://download.pytorch.org/whl/cu118
```

3. For specific chapters, install additional requirements:
- Chapter 5-6: `pip install faiss-gpu`
- Chapter 7: `pip install sacrebleu rouge_score nltk seqeval`
- Chapter 9: `pip install gradio`
- Chapter 10: `pip install argilla`
- Chapter 11: `pip install trl peft`

## Resources

- [Hugging Face Course](https://huggingface.co/course)
- [Transformers Documentation](https://huggingface.co/docs/transformers)
- [Datasets Documentation](https://huggingface.co/docs/datasets)
- [TRL Documentation](https://huggingface.co/docs/trl)
